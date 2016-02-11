#!/usr/bin/env python2.7
# Very very ugly hack.
# Who the fuck needs testing suits!? =)
import sys
import json
import subprocess

def give_call_string(targets, file_):
    test_file = open(file_, 'r')
    test_calls = json.loads(test_file.read())
    test_file.close()

    gdb_call = "break main\n"
    gdb_call += "run\n"

    for target in targets:
        gdb_call += "\n".join(test_calls[target])
    return gdb_call

def get_targets():
    targets = ['default']
    if (len(sys.argv) > 1):
        targets = argv[1:]
    return targets

def run_tests(test_file, binary):
    gdb_call = give_call_string(get_targets(), test_file)
    process = subprocess.Popen(['gdb ' + binary],
                               shell = True,
                               stdout = subprocess.PIPE,
                               stderr = subprocess.PIPE,
                               stdin = subprocess.PIPE)

    result = process.communicate(gdb_call)

    OKGREEN = '\033[92m'
    FAIL = '\033[91m'

    rows = result[1].split('\n')
    for row in rows:
        if row.find('[PASS]') is not -1:
            if row.strip()[6:9] == "...":
                print "\t" + row.strip()[10:]
            else:
                print OKGREEN + row.strip()

        if row.find('[FAIL]') is not -1:
            if row.strip()[6:9] == "...":
                print "\t" + row.strip()[10:]
            else:
                print FAIL + row.strip()



#
# Configure the very basic shit here
#
if __name__ == "__main__":
    test_file = "tests.json"
    binary = "../bin/debug/nightwing"

    run_tests(test_file, binary)





