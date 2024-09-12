import subprocess
for i in range(1, 100, 2):
    result = subprocess.run(["./a.out","100",str(i)],capture_output=True, text=True)
    output = result.stdout
    
    print(f"{i},{output}")

