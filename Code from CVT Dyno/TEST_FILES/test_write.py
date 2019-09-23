from pathlib import Path
directory = "/media/usb1/"
mydir = Path(directory)
if mydir.is_dir():
    print("Directory Exists")
    index = 0
    filename = directory+"file"+str(index)+".csv"
    file = Path(filename)
    while file.is_file():
        index = index+1
        filename = directory+"file"+str(index)+".csv"
        file = Path(filename)
    
    open(str(file),'a')
    fh = open(str(file),'w')
    content = '1521184987.6471033'+','+'\n'+'1521184987.8975418'+','+'\n'+'1521184988.2063005'+','+'\n'+'1521184988.456885'+','+'\n'+'1521184988.7602022'
    fh.write(content)
    fh.close
else:
    print("invalid directory")

