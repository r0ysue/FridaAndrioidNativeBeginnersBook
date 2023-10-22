'''
Generate a platform-dependant script used to register the custom URI handler 'jeb:',
which can be used to execute JEB from other applications.
TODO: only Windows and Linux/Gnome is implemented; add macOS

How to use:
- once the 'jeb:' URI handler is registered, other applications can invoke JEB
- standard JEB parameters must be URL-encoded and provided after the URI
- eg, in your browser: opening the URL 'jeb:https%3A%2F%2Fwww.pnfsoftware.com%2Fz.apk' will 
  invoke JEB with the argument https://www.pnfsoftware.com/z.apk, which JEB will offer you
  to download and process.
'''

import os
import platform
import sys

n = platform.system()
text = None
outname = None

if n == 'Windows':
  template = r'''Windows Registry Editor Version 5.00

[HKEY_CURRENT_USER\Software\Classes\jeb]
"URL Protocol"=""
@="URL:jeb protocol"

[HKEY_CURRENT_USER\Software\Classes\jeb\DefaultIcon]
@="jeb.exe,1"

[HKEY_CURRENT_USER\Software\Classes\jeb\shell]

[HKEY_CURRENT_USER\Software\Classes\jeb\shell\open]

[HKEY_CURRENT_USER\Software\Classes\jeb\shell\open\command]
@="\"%s\" \"%%1\""
'''
  path = os.path.abspath(os.getcwd() + r'\..\..\jeb_wincon.bat')
  text = template % path.replace('\\', '\\\\')
  outname = 'RegisterJebUriScheme.reg'

elif n == 'Linux':
  template = r'''#!/bin/sh
set -uex

if which xdg-mime > /dev/null; then
  DESKTOP_DIR="$HOME/.local/share/applications"
  DESKTOP_PATH="$DESKTOP_DIR/jeb-opener.desktop"

  cat << EOF > "$DESKTOP_PATH"
[Desktop Entry]
Type=Application
Name=JEB decompiler scheme handler
Exec="{jeb_bin_path}" %u
StartupNotify=true
MimeType=x-scheme-handler/jeb;
EOF

  xdg-mime default "$DESKTOP_PATH" x-scheme-handler/jeb
  update-desktop-database "$DESKTOP_DIR"
fi

if which gconftool-2 > /dev/null; then
  gconftool-2 -t string -s /desktop/gnome/url-handlers/jeb/command '"{jeb_bin_path}" "%s"'
  gconftool-2 -s /desktop/gnome/url-handlers/jeb/needs_terminal false -t bool
  gconftool-2 -s /desktop/gnome/url-handlers/jeb/enabled true -t bool
fi
'''
  path = os.path.abspath(os.getcwd() + r'/../../jeb_linux.sh')
  text = template.format(jeb_bin_path=path)
  outname = 'RegisterJebUriScheme.sh'

else:
  print('Not implemented for platform %s!' % n)
  sys.exit(-1)

with open(outname, 'w') as f:
  f.write(text)
print('Execute the generated file %s to register the JEB URI scheme.' % outname)
