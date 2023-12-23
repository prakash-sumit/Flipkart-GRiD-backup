import cv2
import numpy as np

def nothing(x):
    pass

# Create a window
cv2.namedWindow('image')

# create trackbars for color change
cv2.createTrackbar('BMin', 'image', 0, 255, nothing)
cv2.createTrackbar('GMin', 'image', 0, 255, nothing)
cv2.createTrackbar('RMin', 'image', 0, 255, nothing)
cv2.createTrackbar('BMax', 'image', 0, 255, nothing)
cv2.createTrackbar('GMax', 'image', 0, 255, nothing)
cv2.createTrackbar('RMax', 'image', 0, 255, nothing)

# Set default value for MAX BGR trackbars.
cv2.setTrackbarPos('BMax', 'image', 255)
cv2.setTrackbarPos('GMax', 'image', 255)
cv2.setTrackbarPos('RMax', 'image', 255)

# Initialize to check if BGR min/max value changes
bMin = gMin = rMin = bMax = gMax = rMax = 0
pbMin = pgMin = prMin = pbMax = pgMax = prMax = 0

# Open a connection to the webcam (0 represents the default camera)
cap = cv2.VideoCapture(2)

wait_time = 33

while True:
    # Read a frame from the webcam
    ret, frame = cap.read()
    if not ret:
        break

    # get current positions of all trackbars
    bMin = cv2.getTrackbarPos('BMin', 'image')
    gMin = cv2.getTrackbarPos('GMin', 'image')
    rMin = cv2.getTrackbarPos('RMin', 'image')

    bMax = cv2.getTrackbarPos('BMax', 'image')
    gMax = cv2.getTrackbarPos('GMax', 'image')
    rMax = cv2.getTrackbarPos('RMax', 'image')

    # Set minimum and max BGR values to display
    lower = np.array([bMin, gMin, rMin])
    upper = np.array([bMax, gMax, rMax])

    # Create BGR Image and threshold into a range.
    mask = cv2.inRange(frame, lower, upper)
    output = cv2.bitwise_and(frame, frame, mask=mask)

    # Print if there is a change in BGR value
    if (pbMin != bMin) or (pgMin != gMin) or (prMin != rMin) or (pbMax != bMax) or (pgMax != gMax) or (prMax != rMax):
        print("(bMin = %d , gMin = %d, rMin = %d), (bMax = %d , gMax = %d, rMax = %d)" % (bMin, gMin, rMin, bMax, gMax, rMax))
        pbMin = bMin
        pgMin = gMin
        prMin = rMin
        pbMax = bMax
        pgMax = gMax
        prMax = rMax
    

# Set your lower and upper bounds for BGR values
lower_bound = np.array([150, 150, 150], dtype=np.uint8)
upper_bound = np.array([255, 255, 255], dtype=np.uint8)

# Read your image
image = cv2.imread("your_image_path.jpg")  # Replace with your actual image path

# Threshold the image to get a binary mask
mask = cv2.inRange(image, lower_bound, upper_bound)

# Find contours in the binary mask
contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

# Draw contours on the original image
result = image.copy()
cv2.drawContours(result, contours, -1, (0, 255, 0), 2)


# Display output image
cv2.imshow('image', output)

# Wait longer to prevent freeze for videos.
if cv2.waitKey(wait_time) & 0xFF == ord('q'):
    break

# Release the webcam and close all windows
cap.release()
cv2.destroyAllWindows()
