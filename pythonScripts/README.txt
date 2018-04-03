Steps To Setting Up Iotfluorometer:

	Initial Installation:
		1. Download latest version of openwrt for yun here: https://github.com/redsnake64/openwrt-yun/releases . 
		2. Reflash your Yun with the .bin files that you just downloaded. find out how on the official arduino website.
		3. Replace the "%arduinodir%/libraries/bridge" directory with the unzipped "bridge.zip" folder and precompile your python scripts with "python -m compileall" /
		 "python -m compileall /usr/lib/python2.7/bridge".
		4. If you have an SD card or a usb key, expand the storage of your Yun. (I used http://www.ibuyopenwrt.com/index.php/11-yun-compatible/expand-the-storage-at-yun/21-expand-the-storage-iii)

	Cloud Setup:
		1. Create a google developer account or a gmail account if you haven't already.
		2. Go to https://console.developers.google.com/apis/ and create a project.
		3. Search for and enable Google Sheets API in the Library tab.
		4. Go to the Credentials tab and create a Service account key. Make the key of type JSON, and the owner.
		5. Download the JSON. Should look like "NameOfProject-j637hjk576ed.json".
		6. Go to the pythonscripts folder in IOTFluorometer directory and transfer (ex. using scp) the python script "googleSpread.py" to your Yun. Transfer the JSON file you downloaded earlier in the same way.
		7. Create a google spreadsheet.
		8. Share the Spreadsheet with the email account created with the service account (ex: owner-133@NameOfProject.iam.gserviceaccount.com)

	Before Starting:
		1. Run the python script directly from your Yun (remember to replace "value.get("JSON")" in the python script with the path to your JSON file) and download any dependencies you might need.
		 Usually, you'll need to run the following:
		 opkg update
		 opkg install python-openssl
		Then install pip with 
		 wget http://bootstrap.pypa.io/get-pip.py --no-check-certificate
		 python get-pip.py
		With pip you can install the rest of your dependancies 
		 pip install oauth2client
		 pip install gspread
		2. Make sure the in your Fluoremeter.cpp you specify the correct paths to your files and the correct Spreadsheet key.

And you should be ready to go!