alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
resultArray = bytearray()
byteArray = bytearray()
byteString = ''

def base64_encode(inputString):

    byteArray = bytearray(inputString, 'utf-8')
    sumOfBit = len(byteArray)*8   #Here we are finding sum of bits that % 6 == 0, until that we add null bytes
    while(sumOfBit%6 != 0):
        byteArray.append(0)
        sumOfBit+=8

    byteString = ''
    for byte in byteArray:  #append bytes to 8 bits and then convert bytes => string of bytes
        number = "{0:b}".format(byte)
        byteString+= '0'*(8-len(number)) + number

    while(byteString):  #we take every 6 bits and convert them to bytes
        resultArray.append(int(byteString[0:6],2))
        byteString=byteString[6:]

    result=''
    for byte in resultArray:    #then we are building string from our alphabet
        if byte==0:
            result += '='
        else:
            result += alphabet[byte]

    return result

def base64_decode(inputString):
    for letter in inputString:
        if letter=='=':
            byte=0
        else:
            byte = alphabet.index(letter)
        byteArray.append(byte)

    byteString=''
    for byte in byteArray:
        number = "{0:b}".format(byte)
        byteString+='0'*(6-len(number)) + number

    while(byteString):
        resultArray.append(int(byteString[0:8],2))
        byteString=byteString[8:]

    return resultArray.decode("utf-8")

def main():
    print("Welcome to base64 encoder/decoder. Input your string:",end=' ')
    string = input()
    print("1) encode\n2) decode\nWhat do you want:",end=' ')
    ans = input()
    if ans=='1':
        print("Encoded string: " + base64_encode(string))
    elif ans=='2':
        print("Decoded string: " + base64_decode(string))
    else:
        print("Bad answer, exiting.")

if __name__ == "__main__":
    main()
