# Example of showing Mailbox 5 fifo status:
set -o xtrace
echo "MAILBOX_REVISION"
omapconf read 0x48840000
echo "MAILBOX_FIFOSTATUS_m4"
omapconf read 0x48840090
echo "MAILBOX_MSGCOUNT_m4"
omapconf read 0x488400D0
#echo "MAILBOX_MESSAGE_m4"
#omapconf read 0x48840050

echo "MAILBOX_FIFOSTATUS_m6"
omapconf read 0x48840098
echo "MAILBOX_MSGCOUNT_m6"
omapconf read 0x488400D8
#echo "MAILBOX_MESSAGE_m6"
#omapconf read 0x48840058

