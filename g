#! /usr/bin/python
import socket
import subprocess
import pickle


def run(cmd):
        print cmd
        proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, shell=True)
        (out, err) = proc.communicate()
        print out
        return out

s = socket.socket()
host = '0.0.0.0'
port = 44445
s.bind((host, port))
s.listen(5)

run('/etc/init.d/networking restart')
run('ping -c 10 10.129.28.187')



while True:
        c, addr = s.accept()
        cmds = c.recv(1024)
        cmd=pickle.loads(cmds)
#       print cmd
        for each in cmd:
                out = run(each)
                c.send(out)
        out=run('echo ENDDELIMETER')
        #sleep(5)
        c.send(out)

c.close()
