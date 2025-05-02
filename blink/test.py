

def fibonacci():
    fibo = [0 for _ in range(20)]
    fibo[0] = 0
    fibo[1] = 1

    for i in range(2, 20):
        fibo[i] = fibo[i-1] + fibo[i-2]

    print(fibo)

    pointer = 3

    for i in range(0, 1000):
        if i == fibo[pointer]:
            print(f"{i}: Blink")
            pointer += 1

fibonacci()