import re
import getpass
end_marker = "#"
current_line = str()
frame_hex = str()

print('Paste Wireshark CoAP frame as HEX dump and type # at the end:')
while end_marker not in current_line:
    current_line = '\n' + getpass.getpass('')
    frame_hex += current_line


raw_result = re.findall(r'\b([a-fA-F0-9]{2})\b', frame_hex)
hex_result = ['0x' + x for x in raw_result]

print('unsigned int buffer_size = ' + str(len(hex_result)) + ';')
print('unsigned char buffer[] = {' + ', '.join(hex_result) + '};')
