
def cortarlinea(l):
    i=0
    arr=['','','']
    while l[i]!=' ':
        i=i+1
    i=i+1
    while l[i]!=' ':
        i=i+1
    arr[0]=l[0:i]
    aux = i+1
    i=i+1
    while l[i]!=' ':
        i=i+1
    arr[1]=l[aux:i]
    aux = i+1
    arr[2]=l[aux:len(l)]
    return arr

print(cortarlinea('2022-11-29 20:34:55 340 70'))
                
        