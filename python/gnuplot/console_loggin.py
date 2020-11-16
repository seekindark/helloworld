### Module with common function for whole script, diag class and init root logger
import logging
import os
import socket
import collections
import threading
import re
import struct
import sys
from time import sleep
from logging.handlers import RotatingFileHandler

#Class, which handle connection to node console
class Diag:

    COMMAND_TIMEOUT = 30 # in seconds

    class CommandTimeoutException(Exception):
        """Exception is thrown when expected command output is not found"""

        def __init__(self):
            super(Diag.CommandTimeoutException, self).__init__("Command timeout")

    def __init__(self, **kwargs):
        self.TCP_IP = kwargs['host'] if kwargs and 'host' in kwargs else "127.0.0.1"
        self.TCP_PORT = kwargs['port'] if kwargs and 'port' in kwargs else 55555
        self.USER = kwargs['user'] if kwargs and 'user' in kwargs else "diag"
        self.BUFFER_SIZE = 1024*100
        self.enter_on_continue = True
        self.wait_timeout = 0
        self.time_from_last_command = 1
        self.rconsole_wait = False
        
        # wait for vscm up
        sleep(1)
        
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((self.TCP_IP, self.TCP_PORT))
        self._send_handshake_msg()

        self.rcvbuf = collections.deque(maxlen=10)
        self.rseqid = 0
        self.rcvtrh = threading.Thread(target=self._rcv_thr_main)
        self.rcvtrh.setDaemon(True)
        self.rcvtrh.start()
        self.keepalivetrh = threading.Thread(target=self._keep_alive)
        self.keepalivetrh.setDaemon(True)
        self.keepalivetrh.start()

    def __to_bytes(self, n, length, endianess='little'):
        h = '%x' % n
        s = ('0'*(len(h) % 2) + h).zfill(length*2).decode('hex')
        return s if endianess == 'big' else s[::-1]
        
    def _rcv_thr_main(self):

        chunks = []
        self.socket.settimeout(2)
        while 1:
            try:
                chunk = self.socket.recv(self.BUFFER_SIZE)
                if chunk == '':
                    raise RuntimeError("socket connection broken")
            except socket.timeout:
                sleep(1)
                continue
            except:
                sleep(10)
                self._reconnect()
                continue

            chunks.append(chunk)
            last_line = chunk.splitlines()[-1]
            if re.search(r"continue\:", last_line, re.IGNORECASE):
                if self.enter_on_continue:
                    self.socket.send("\n")
                else:
                    self.socket.send("q\n")
            elif re.search(r"continue\? \(y\/n\)", last_line):
                self.socket.send("y\n")
            #match # instead of vMGx# to match some special case
            elif (not self.rconsole_wait and re.search(r"#\s+", last_line)) or \
                (self.rconsole_wait and re.search(r"rem\-cons.*#\s+", last_line)):
                # this is: simplex-bgw1:ACT-SCM:1.10(r0)>=2:diag:main:vMGx# or
                # simplex-bgw1:rem-cons:ACT-SCM:1.10(r0)>=2:diag:main:vMGx#
                self.rcvbuf.append(''.join(chunks))
                self.rseqid += 1
                chunks = []
            else:
                # each receive clear timeout value
                self.wait_timeout = 0
                
    def _reconnect(self):
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.TCP_IP, self.TCP_PORT))
            self._send_handshake_msg()
        except:
            pass

    def _keep_alive(self):
        while True:
            sleep(1)
            try:   
                #after 60 seconds after last command send keep alive
                if self.time_from_last_command % 60 == 0:
                    self.socket.send("\n")
                self.time_from_last_command += 1
            except:
                pass

    def _send_handshake_msg(self):
        '''
        refer to
         - profile_info_t struct defintion (mshell.h)
         - msg_header_t struct definition (pamtask.h)
         - mshell.c to find how to establish connection
        '''
        socket_info = self.socket.getsockname()

        format = '32s H H 16s i B i 16x'
        profile = struct.pack(format,
                              self.USER,
                              2,  # AF_INET     2   /* Internet IP Protocol     */
                              socket_info[1],
                              socket.inet_aton(socket_info[0]),
                              os.getpid(),
                              2,  # MSHELL_SSH_AUTH_PASSWORD   = 2,
                              0  # SSH type
                              )

        hmsg = bytearray([0x10, 0x30])
        hmsg.extend(self.__to_bytes(struct.calcsize(format), 2))
        hmsg.extend(profile)
        self.socket.send(hmsg)

    def scm_disconnect(self):
        self.socket.close()

    def run_command(self, cmd, enter_on_continue=True):

        """Method, which send command to socket

            Args:
                cmd (str): node console command
                enter_on_continue (bool): if true automatically print enter if continue keyword detected

            Returns:
                str: command output

        """

        id = self.rseqid
        self.enter_on_continue = enter_on_continue
        self.wait_timeout = 0
        self.time_from_last_command = 1
        # special expected end of command
        self.rconsole_wait = True if "rconsole" in cmd else False
        wait_sleep = 0.5
        cmd_exec_time = 0
        
        try:
            self.socket.send(cmd + "\n")
        except:
            raise Exception("Cannot send data to the socket")
        
        # wait till we get something in resp buffer
        #before this while, DO NOT send any command, that will increase rseqid
        while self.rseqid == id:
            self.wait_timeout += wait_sleep
            cmd_exec_time += wait_sleep
            # print dot every 10 seconds
            if not cmd_exec_time % 10:
                sys.stdout.write(".")
                sys.stdout.flush()
            if self.wait_timeout > Diag.COMMAND_TIMEOUT:
                self._reconnect()
                raise Diag.CommandTimeoutException()
                #return ""
            sleep(wait_sleep)

        return self.rcvbuf[-1]

    def run_commands(self, cmds):
        '''
        execute multiple commands
        '''
        out = []
        for cmd in cmds:
            #self.__logger.debug("processing: %s", cmd)
            out.append({"cmd": cmd, "output": self.run_command(cmd)})
            # to keep incompleted cmd return normal, need another enter key
            self.run_command("")
        return out

def init_logger(logfile):

    """Function, which initialize root logger.
        Its config root logger, set rotating file handler

        Args:
            logfile (str): log file path
    """

    logger = logging.getLogger("")
    logger.setLevel(logging.INFO)
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    rotfilehandler = RotatingFileHandler(logfile, backupCount=10)
    
    if os.path.isfile(logfile):
        rotfilehandler.doRollover()

    rotfilehandler.setLevel(logging.INFO)
    rotfilehandler.setFormatter(formatter)
    logger.addHandler(rotfilehandler)
