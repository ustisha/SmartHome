#!/usr/bin/env bash

CWD="$(pwd)"
SCRIPT_DIR="$CWD/../../src"

/usr/bin/php "$SCRIPT_DIR/Ws.php" stop
/usr/bin/php "$SCRIPT_DIR/Tcp.php" stop
