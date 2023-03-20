import numpy as np
import cv2
import os

def toGrayScale():

    grayScales = []
    netGrayScales = []

    for colors in netColors:

        grayScales = []
        for channel in colors:

            # B G R -> W% correct luminance for visual biases.

            grayVal = (channel[0] + channel[1] + channel[2])
            grayScales.append(grayVal)

        netGrayScales.append(grayScales)
    
    return netGrayScales


def gridAndShow():

    imagesOut = []
    gsParam = 5
    for item in netGrayScales:

        img = np.zeros((1170, 1170), dtype=np.uint8)
        for i in range(gridsize[0]):
            for j in range(gridsize[1]):
                value = item[i*gridsize[0]+j]  # Scale down the values to fit in 8-bit image
                img[i*gsParam:i*gsParam+gsParam, j*gsParam:j*gsParam+gsParam] = value
        
        imagesOut.append(img)

    return imagesOut
        
def writeToDisk():

    print("\nWriting Images...\n")
    i=0

    folder_path = 'output'
    if not os.path.exists(folder_path):
        os.makedirs(folder_path)

    raw_folder_path = 'raw_output'
    if not os.path.exists(raw_folder_path):
        os.makedirs(raw_folder_path)
    
    for img in imagesOut:

        tentName = str(times[i])+'h'+'00.png'
        tentName = os.path.join(folder_path, tentName)

        cv2.imwrite(tentName, img)

        i+=1
        print(str(i)+'/'+str(len(imagesOut)))

    i=0
    for raw in rawImagesOut:

        tentName = str(times[i])+'h'+'00_raw.png'
        tentName = os.path.join(raw_folder_path, tentName)

        cv2.imwrite(tentName, raw)

        i+=1

def getRawSunny():
    
    absMAX = 0
    rawCounts = []
    thresh = 150
    
    for gray in netGrayScales:
        if max(gray) > absMAX:
            absMAX = max(gray)

    imagesOut = []
    gsParam = 5
    for item in netGrayScales:

        rawCnt = 0
        img = np.zeros((1170, 1170), dtype=np.uint8)
        for i in range(gridsize[0]):
            for j in range(gridsize[1]):

                value = item[i*gridsize[0]+j]  # Scale down the values to fit in 8-bit image

                if value>thresh:
                    value = 255
                    rawCnt+=1
                else:
                    value = 0
                
                img[i*gsParam:i*gsParam+gsParam, j*gsParam:j*gsParam+gsParam] = value

        rawCounts.append(rawCnt)
        
        imagesOut.append(img)

    return imagesOut, rawCounts

# def iterateTimes():

#     i = 0
#     for t in times:
#         if(t>=10 and t<=12):
#             insomniacType.append(0.2)
#         else:



gridsize = (234,234)
images = ('IMG_2756', 'IMG_2756', 'IMG_2757', 'IMG_2758', 'IMG_2759', 'IMG_2760', 'IMG_2761', 'IMG_2762', 'IMG_2763', 'IMG_2764', 'IMG_2765', 'IMG_2766', 'IMG_2766')
path = 'photos/'
ext = '.png'
times = []
paths = []

maxSunny = 22222

cloudCover = [20,20,3,1,0,5,0,13,29,29,18,4,3]
inverseCloud = []

insomniacType = []

for per in cloudCover:
    inverseCloud.append(1-(per/100))

i=0
for x in images:
    times.append(2*i)
    paths.append(str(path+x+ext))
    
    i+=1

imreadimages = []
hsvs = []
i = 0

bgrs = []

for x in images:

    imreadimages.append(cv2.imread(paths[i], cv2.IMREAD_GRAYSCALE))
    bgrs.append(cv2.cvtColor(imreadimages[i], cv2.COLOR_GRAY2BGR))
    hsvs.append(cv2.cvtColor(bgrs[i], cv2.COLOR_BGR2HSV))

    i += 1

cells = []
cWidths = []
cHeights = []

i = 0

for h in hsvs:
    cells.append(cv2.split(h))
    cHeights.append(cells[i][0].shape[0] // gridsize[0])
    cWidths.append(cells[i][0].shape[1] // gridsize[1])

    i += 1

netColors = []

for m in range(len(hsvs)):

    colors = []
    avgColors = []
    for i in range(gridsize[0]):
        for j in range(gridsize[1]):
            x = j * cWidths[m]
            y = i * cHeights[m]
            cells[m] = hsvs[m][y:y+cHeights[m], x:x+cWidths[m]]
            avgColors = cv2.mean(cells[m])
            colors.append(avgColors)

    netColors.append(colors)

netGrayScales = toGrayScale()
imagesOut = gridAndShow()
rawImagesOut, rawCounts = getRawSunny()

# iterateTimes()
finalList = []
for raw in rawCounts:
    finalList.append(raw/(maxSunny))

timesReadable = []
for t in times:

    if t<10:
        timesReadable.append('0'+str(t)+":"+"00")
    else:
        timesReadable.append(str(t)+":"+"00")

writeToDisk()

finalList = [round(elem, 3) for elem in finalList]

print("\nThe scores are as follows:")
print(finalList)

i=0
for final in finalList:
    finalList[i] = finalList[i]*inverseCloud[i]
    i+=1

finalList = [round(elem, 3) for elem in finalList]

print("\nAccounting for Clouds and other Factors:")
print(finalList)

print("\nFor times:")
print(timesReadable)
print("\n")
