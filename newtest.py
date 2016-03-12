import subprocess as sub
import time, os, string

lines = [line.rstrip('\n') for line in open('/home/hackpi/wifi_networks-01.csv')]
del lines[0]
del lines[0]

outfile=open('/home/hackpi/wep_networks', 'w')

for sline in lines:
 mline= str.split(sline) 
 if len(mline) > 7: 
  if (mline[7] == "WEP") or (mline[7] == "WEP,"):
   outfile.write(" ".join(mline))
   chan=mline[5][:-1]
   bssid=mline[0][:-1]
   print bssid
#  sub.call(['airmon-ng', 'start', 'wlan2'])
   sub.Popen(['airodump-ng', '--bssid='+bssid, '--channel='+chan, '--write=/home/hackpi/wep_network_'+bssid, '--output-format=csv', '--output-format=pcap', 'wlan2mon' ])
  
