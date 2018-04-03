#!/usr/bin/python

import sys
import json
import gspread
from oauth2client.service_account import ServiceAccountCredentials

scope = ['https://spreadsheets.google.com/feeds']

credentials = ServiceAccountCredentials.from_json_keyfile_name('YunToCloud-a9572c8560ed.json', scope)
#credentials = SignedJwtAssertionCredentials(json_key['poulio445@gmail.com'], json_key['a9572c8560edf0ad1ff917066b3c7185094d7491'], scope)

gc = gspread.authorize(credentials)
sh = gc.open_by_url('https://docs.google.com/spreadsheets/d/1cgSD0VVSioqxxj_yS5CB060Dc6318NMllCBwDQ79DzI/edit#gid=0')
wks= sh.get_worksheet(0)
z = int(sys.argv[1])
for x in range(1, 20):
	y = 'A' + str(x)
	wks.update_acell(y, str(z))
	z += 100