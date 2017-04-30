import subprocess

def average(list):
	return sum(list)/len(list)

results = []
runNumber = 3
simLength = 1000

serial = './a.out'
gpu = './a.out'

#gpu = './gpu'

#./a.out  
# we could use this to run a problem size detection graph as well 

serialSpeed = 0; 
gpuSpeed = 0;

for i in range(0,runNumber):	
	data = subprocess.check_output([serial,str(simLength)])
	results.append(float(data))
	#print(results)
serialSpeed = average(results)
results = [] 

for i in range(0,runNumber):	
	data = subprocess.check_output([gpu,str(simLength)])
	results.append(float(data))
	#print(results)
gpuSpeed = average(results)

print("Speedup: "+str(serialSpeed/gpuSpeed))



