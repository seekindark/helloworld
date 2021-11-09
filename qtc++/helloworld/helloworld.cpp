#include<QApplication>
#include<QLabel>
#include<QDialog>

int main(int argc,char* argv[]){
    QApplication a(argc, argv);
    QDialog w;
    w.resize(300, 200);
    QLabel label(&w);
    label.move(100,50);
    label.setText(QObject::tr("Hello, Kunming"));
    w.show();
    return a.exec();
}