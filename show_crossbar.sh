# Run this on target:
set -o xtrace
echo "CTRL_CORE_MPU_IRQ_136_137: expect 0xFB"
omapconf read 0x4A002B48
echo "CTRL_CORE_MPU_IRQ_140_141: expect 0xFF"
omapconf read 0x4A002B52
