#!/usr/bin/python
import sys
import json
import gspread
import time
import urllib3.contrib.pyopenssl

urllib3.contrib.pyopenssl.inject_into_urllib3()
from oauth2client.client import GoogleCredentials

credentials = GoogleCredentials.get_application_default()
credentials = credentials.create_scoped(['https://spreadsheets.google.com/feeds'])

#credentials = ServiceAccountCredentials.from_json_keyfile_name('YunToCloud-a9572c8560ed.json', scope)
#credentials = SignedJwtAssertionCredentials(json_key['poulio445@gmail.com'], json_key['a9572c8560edf0ad1ff917066b3c7185094d7491'], scope)

gc = gspread.authorize(credentials)
sh = ''
while sh == '':
        try:
                sh = gc.open_by_key('1cgSD0VVSioqxxj_yS5CB060Dc6318NMllCBwDQ79DzI').sheet1
        except:
                time.sleep(5)
                continue
wks= sh.get_worksheet(0)
z = int(sys.argv[1])
for x in range(1, 1):
        y = 'A' + str(x)
        wks.update_acell(y, str(z))
        z += 100
