#============================================================================#
#============================================================================#
#   translate radio Msq from byte to int
#============================================================================#
def translate_from_radio(msg, size, debug=False):
    translated_msg=[]
    for i in range (0, size, 4):
        translated_msg.append(int.from_bytes([msg[i+3], msg[i+2], msg[i+1], msg[i]], byteorder='big')) 
        
    if (debug):
        print("Translate FROM Radio: " + str(msg) + " --> " + str(translated_msg))
    return translated_msg
#============================================================================#
#============================================================================#
#   Split the msg element in 4 bytes and add it to translated msg
#============================================================================#   
def translate_to_radio(msg, debug=False):
    translated_msg=[]
    for i in range (0, len(msg)):
        x=msg[i].to_bytes(4, byteorder='big')
        for g in reversed(x):
            translated_msg.append(g)        
    if (debug):    
	    print("Translate TO Radio: " + str(msg) + " --> " + str(translated_msg))
    return translated_msg
#============================================================================#
#============================================================================#
#   Seperates the Clip ID and the Task | idTask[ID, Task] ID = 22 * 1000 + Task
#============================================================================#
def idTask (idTask):
    if type(idTask) is int:
        create_task = int(idTask%1000)
        create_id = int((idTask-create_task)/1000)
        new = [create_id, create_task]
        return new
		
    if type(idTask) is list:
        new = idTask[0] * 1000 + idTask[1]
        return new
        