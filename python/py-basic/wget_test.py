#! /usr/bin/python3

print("preparing to download the picture...")
import wget
url = "https://img1.doubanio.com/lpic/s28735609.jpg"
out = "E:\\机器学习.jpg"

print("started to download the picture...")
wget.download(url, out=out)

print("\n\n...Done!")
