#!/usr/bin/python
import os
import sys

def read_data(filename):
    t_list = []
    fo = open(filename, "r")
    section = ""
    key = ""
    for line in fo.readlines():
        key = ""        
        line = line.rstrip("\n")
        if line == "":
            continue        

        if line[0] == "#":
            t_list.append(line)
            continue
        elif "]" in line:
            section = line
            t_list.append(section)
            continue
        elif "=" in line:
            key = section + line
            t_list.append(key)
            continue
        else: 
            print("不可识别的行:" + line)
            fo.close()
            return []
    fo.close()            
    return t_list

def merge(old_filename, new_filename):
    old_list = []
    new_list = []
    old_list = read_data(old_filename)
    new_list = read_data(new_filename)

    if old_list == [] or new_list == []:
        print("获取文件数据失败")
        return False

    for val in  old_list[:]:
        
        if val not in new_list:
            if val[0] == "#":
                continue
            elif "]" in val and "[" in val and "=" not in val:
                idx = old_list.index(val)
                if idx > 0:
                    if old_list[idx - 1][0] == "#":
                        del old_list[idx - 1]
                old_list.remove(val)                      
            elif "]" in val and "[" in val and "=" in val:
                idx = old_list.index(val)
                if idx > 0:
                    if old_list[idx - 1][0] == "#":
                        del old_list[idx - 1]
                old_list.remove(val) 
            else:
                pass

    for val in new_list[:]:
        if val not in old_list:
            if val[0] == "#":
                continue                
            elif "]" in val and "[" in val and "=" not in val:
                idx = new_list.index(val)
                if idx > 0:
                    if new_list[idx - 1][0] == "#":
                        old_list.append(new_list[idx - 1])
                old_list.append(val)
            elif "]" in val and "[" in val and "=" in val:
                idx = val.find("]")
                sec = val[0:idx+1]
                sec_idx = old_list.index(sec)
                    
                idx = new_list.index(val)
                if idx > 0:
                    if new_list[idx - 1][0] == "#":
                        old_list.insert(sec_idx + 1,new_list[idx - 1])
                        old_list.insert(sec_idx + 2, val)
                        continue
                old_list.insert(sec_idx + 1, val)
            else:
                pass


    fo = open("merge.ini", "w")

    for val in  old_list:
        if val[0] == "#":
            fo.write(val + "\n")
            continue
        elif  "]" in val and "[" in val and "=" not in val:
            fo.write(val + "\n")
            continue
        elif  "]" in val and "[" in val and "=" in val:
            idx = val.find("]")
            fo.write(val[idx+1:] + "\n")
            continue
        else:
            pass

    fo.close()            
    return True        


merge("1.ini", "2.ini")


'''
def read_data(filename):
    t = {}
    fo = open(filename, "r")
    section = ""
    key = ""
    val = []
    for line in fo.readlines():
        line.rstrip("\n")
        key = ""
        val = []

        if line[0] == "#":
            continue

        if "]" in line:
            line = line.rstrip("\n")
            section = line
        elif "=" in line:
            line = line.rstrip("\n")
            idx = line.find("=", 0, len(line))
            key = section + line[0:idx]
            val.append(line[idx + 1:])
            val.append(0)
        else: 
            pass

        if len(key) != 0:
            t[key] = val

    fo.close()            
    return t

def merge(old_filename, new_filename):
    old = read_data(old_filename)
    new = read_data(new_filename)

    for key,val in  new.items():
        if key in old:
            t = old.get(key)
            t[1] = 1
            old[key] = t
        else:
            val[1] = 1
            old[key] = val
         
    fo = open("merge.ini", "w")

    t = {}
    for key,val in  old.items():
        
        if val[1] != 0:
            idx = key.find("]", 0, len(key))
            if idx == -1:
                return False

            section = key[0:idx+1]
            _key = key[idx + 1:]
            if section not in t:
                t[section] = 1
                fo.write(section+"\n")
            fo.write(_key + "=" +val[0] +"\n")
    fo.close()            
    return True        


merge("1.ini", "2.ini")
'''




'''
def read_data(filename):
    t = []
    return t


def merge(old_filename, new_filename):
    old = read_data(old_filename)
    new = read_data(new_filename)


    for key,val in  new.items():
        if key in old:
            t = old.get(key)
            t[1] = 1
            old[key] = t
        else:
            val[1] = 1
            old[key] = val
    fo = open("merge.ini", "w")

    for key,val in  old.items():
        if val[1] != 0:
            fo.write(val[0])


    fo.close()            
    return True    
'''    