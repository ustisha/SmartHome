#!/usr/bin/env bash

CWD="$(pwd)"
SCRIPT_DIR="$CWD/../../src"

/usr/bin/php "$SCRIPT_DIR/Tcp.php" start -d
/usr/bin/php "$SCRIPT_DIR/Ws.php" start -d
