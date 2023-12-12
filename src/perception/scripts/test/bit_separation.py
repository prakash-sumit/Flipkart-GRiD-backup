# data1 = 2374
# data2 = -226
# data3 = -356
# data4 = 1

# # Pack the data into a single Int64
# packed_data = (data1 << 27) | (data2 << 15) | (data3 << 3) | data4

# print(packed_data)

# # data1 = (packed_data >> 27) & 0xFFFF
# # data2 = (packed_data >> 15) & 0xFFFF
# # data3 = (packed_data >> 3) & 0xFFFF
# # data4 = packed_data & 0xFFFF

# data4= packed_data % (2**3) 
# packed_data= packed_data - data4

# packed_data = packed_data >> 3
# data3 = packed_data % (2**12) - (2**12)
# packed_data= packed_data - data3

# packed_data = packed_data >> 15
# data2 = packed_data % (2**12) 


 
# # Print the unpacked values
# print(f"Data 1: {data1}")
# print(f"Data 2: {data2}")
# print(f"Data 3: {data3}")
# print(f"Data 4: {data4}")

task = int(input('Enter task '))
steps_x = int(input('Enter steps_x '))
steps_y = int(input('Enter steps_y '))
steps_z = int(input('Enter steps_z '))
x,y,z = '','',''
t = str(task)
if(steps_x < 0):
    m = str(abs(steps_x))
    x = '1'+'0'*(4-len(m))+m
else:
    m = str(abs(steps_x))
    x = '0'+'0'*(4-len(m))+m
if(steps_y < 0):
    m = str(abs(steps_y))
    y = '1'+'0'*(4-len(m))+m
else:
    m = str(abs(steps_y))
    y = '0'+'0'*(4-len(m))+m
if(steps_z < 0):
    m = str(abs(steps_z))
    z = '1'+'0'*(4-len(m))+m
else:
    m = str(abs(steps_z))
    z = '0'+'0'*(4-len(m))+m
encoded_string = t+x+y+z
print(encoded_string, len(encoded_string))
encoded_number = int(encoded_string)
print(encoded_number, type(encoded_number))

