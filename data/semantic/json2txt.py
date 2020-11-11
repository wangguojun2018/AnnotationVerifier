'''
Authors: Guojun Wang
Date: 1970-01-01 08:00:00
LastEditors: Guojun Wang
LastEditTime: 2020-11-11 14:12:57
'''
import os
import json
import os
import glob
import numpy as np
import argparse

if __name__=="__main__":

    parser = argparse.ArgumentParser(description='Convert json to txt')
    parser.add_argument('--json-file', help='json label file')
    parser.add_argument('--txt-dir', help='the directory of txt label')
    args=parser.parse_args()

    if not os.path.exists(args.txt_dir):
        os.makedirs(args.txt_dir)

    with open(args.json_file,'r') as f:
        anno_files=json.load(f)
    for annos in anno_files:
        clsname, xlist, ylist, zlist,wlist,llist,hlist,yawlist = [], [], [], [],[],[],[],[]
        filename=annos['name']
        for anno in annos['annotations']:
            clsname.append('Truck')
            xlist.append(np.round(float(anno['geometry']['position']['x']),decimals=4))
            ylist.append(np.round(float(anno['geometry']['position']['y']), decimals=4))
            zlist.append(np.round(float(anno['geometry']['position']['z']), decimals=4))
            wlist.append(np.round(float(anno['geometry']['dimensions']['y']), decimals=4))
            llist.append(np.round(float(anno['geometry']['dimensions']['x']), decimals=4))
            hlist.append(np.round(float(anno['geometry']['dimensions']['z']), decimals=4))
            yawlist.append(np.round(float(anno['geometry']['rotation']['z']), decimals=4))

        occluded=np.zeros((len(clsname),1))
        alpha=np.zeros((len(clsname),1))
        clsname=np.array(clsname)[:,np.newaxis]
        xlist=np.array(xlist)[:,np.newaxis]
        ylist=np.array(ylist)[:,np.newaxis]
        zlist=np.array(zlist)[:,np.newaxis]
        wlist=np.array(wlist)[:,np.newaxis]
        llist=np.array(llist)[:,np.newaxis]
        hlist=np.array(hlist)[:,np.newaxis]
        yawlist=np.array(yawlist)[:,np.newaxis]
        yawlist=yawlist*180/np.pi+90
        idx=np.where(yawlist>180)[0]
        yawlist[idx]=yawlist[idx]-360
        idx_=np.where(yawlist<-180)[0]
        yawlist[idx_] = yawlist[idx_]+360
        yawlist=yawlist*np.pi/180
        truncated = np.zeros((len(clsname), 1))
        boxes2d = np.zeros((len(clsname), 4), dtype=np.float)
        scores=np.ones((len(clsname),1),dtype=np.float)
        all_label = np.concatenate([clsname,truncated,occluded,alpha,boxes2d,wlist,llist,hlist,xlist,ylist,zlist,yawlist,scores],
                                   axis=1)
        with open(os.path.join(args.txt_dir,os.path.splitext(filename)[0]+".txt"), 'w') as f:
            for line in all_label:
                # print(line)
                line = str(line).strip('[').strip(']').replace("'", "").replace("\n", "")
                print(line)
                f.write(line + '\n')
        print("convert {} successful".format(filename))



