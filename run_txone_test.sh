#!/bin/bash

# Start nettest.py in background
python3 nettest.py txone &
NTPID=
sleep 2

# Run xv6 with pty
python3 -c "
import pty, os, time, select, sys

pid, fd = pty.fork()
if pid == 0:
    os.chdir(\"/home/wangpeicong/xv6-labs-2025\")
    os.execvp(\"make\", [\"make\", \"qemu\", \"CPUS=1\"])
else:
    output = b\"\"
    time.sleep(8)
    os.write(fd, b\"nettest txone\n\")
    deadline = time.time() + 15
    while time.time() < deadline:
        r, _, _ = select.select([fd], [], [], 1)
        if r:
            try:
                data = os.read(fd, 4096)
                if data:
                    output += data
                    sys.stdout.write(data.decode(\"utf-8\", errors=\"replace\"))
                    sys.stdout.flush()
                    if b\"halt\" in output.lower() or b\"panic\" in output:
                        break
            except:
                break
    os.write(fd, b\"halt\n\")
    time.sleep(1)
    try:
        while True:
            r, _, _ = select.select([fd], [], [], 0.5)
            if r:
                data = os.read(fd, 4096)
                if not data: break
            else:
                break
    except:
        pass
    os.close(fd)
    os.waitpid(pid, 0)
"

wait  2>/dev/null
echo "=== TXONE TEST DONE ==="
