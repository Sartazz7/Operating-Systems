gcc testcase1.c emufs-disk.c emufs-ops.c
./a.out > output1
diff output1 ./expected_outputs/output1

gcc testcase2.c emufs-disk.c emufs-ops.c
./a.out > output2
#input a key for encryption
diff output2 ./expected_outputs/output2

gcc testcase3.c emufs-disk.c emufs-ops.c
./a.out > output3
diff output3 ./expected_outputs/output3

gcc testcase4.c emufs-disk.c emufs-ops.c
./a.out > output4
# input 2 keys for 2 encrypted devices
diff output4 ./expected_outputs/output4

rm disk*