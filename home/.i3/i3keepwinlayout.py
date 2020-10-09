#!/usr/bin/env python2
# -*- coding: utf-8 -*-

from Xlib import display, X, Xatom
import subprocess
import signal, os, sys
import time

class X11WinWatch:
    cwlay = {}
    curclass = "undef"
    pidfile = sys.argv[0] + '.pid'
    def __init__(self):
        self.display = display.Display()
        self.root = self.display.screen().root
        self.root.change_attributes(event_mask=(X.PropertyChangeMask))
        self.ACTIVE = self.display.intern_atom("_NET_ACTIVE_WINDOW")
        self.display.flush()

        self.activeWindow = self.root.get_full_property(self.ACTIVE, 0).value[0]
        self.doActiveWindow()
#        self.run()
        
    def getKBLayout(self):
        output = subprocess.Popen(["setxkbmap -query | awk ' /layout/ { print $2 }'"], shell=True, stdout=subprocess.PIPE).communicate()[0]
        clay=output.replace("\n", "")
        return clay

    def saveKBLayout(self):
        cc=self.getKBLayout()
        print('--- saving kb layout for current window class:',self.curclass,cc)
        self.cwlay[self.curclass] = cc

    def checkRestoreKBLayout(self):
        print('--- Doing window changed actions')
        window = self.display.get_input_focus().focus
        wmname = window.get_wm_name()
        wmclass = wmclass = window.get_wm_class()
        clay=self.getKBLayout()
        print ("--- Current system layout is ", clay) 
        if not wmclass is None:
             self.curclass = wmclass[1]
             self.cwlay.setdefault(self.curclass,"us")
             print("Current is [",clay,"]",self.curclass)
             wlay=self.cwlay[self.curclass]
             self.cwlay[self.curclass] = "us" if not wlay else wlay
             print("Kept_ here [",wlay,"]",self.curclass)
             if not wlay == clay:
                 self.cwlay[self.curclass]=clay
                 print("need to switch")
                 togglecmd=os.environ['HOME'] + '/.i3/toggle_layout ' + wlay
                 print("Command to run for toggle is"  + togglecmd)
                 subprocess.call(togglecmd, shell=True)

        
    def doActiveWindow(self):
        active = self.root.get_full_property(self.ACTIVE, 0).value[0]
        if active != self.activeWindow:
            print('window change detected ...')
            self.activeWindow = active
            try:
            	self.checkRestoreKBLayout()
            except:
                print('Exception whne restore KB layout')
        
    def run(self):
        self.cwlay.setdefault(self.curclass,"us")
        self.pid=os.getpid()
        with open(self.pidfile, 'w') as f:
            # print(self.pid, file=f)
            print >>f, self.pid
        while 1:
            #print self.pid,self.curclass, "curclass, its layout is ", self.cwlay[self.curclass]
            while self.display.pending_events():
                e = self.display.next_event()
                if e.type == X.PropertyNotify:
                    self.doActiveWindow()                    
            time.sleep(0.1)




print("Creating a")
a=X11WinWatch()

def handler(signum,frame):
     print("Signal handler caught")
     a.saveKBLayout()
 
signal.signal(signal.SIGUSR1, handler)

print("Starting a")
a.run()
print("Out of sight")

## 
## 
