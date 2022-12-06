#practice of chapter3 time:2022_3_9
#列表简介

gest = ['little_red','little_balck','little_yellow']
print(gest)
print(f"{gest[0]} can not come")
gest[0] = 'little_chen'
print(gest)
gest.insert(0,'little_liu')
gest.insert(3,'candy')
gest.append('susi')
print(gest[-1])

gest.pop()
gest.pop()
gest.pop()
gest.pop()
print(gest)
del gest[0]
del gest[0]
print(gest)

num = [0,7,1,4,6,5,2,8,9,3,10]

print(sorted(num))
num.reverse()
print(num)
print(len(num))
