import cv2
import numpy as np

def nothing(x):
    pass

# Load the image
image = cv2.imread('/home/sedrica/Pictures/b.png')  # Replace with the path to your image

# Create a window
cv2.namedWindow('image')

# Set minimum and max BGR values to display
lower = np.array([0, 0, 0])
upper = np.array([179, 26, 255])

# Create HSV Image and threshold into a range.
hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
mask = cv2.inRange(hsv, lower, upper)
output = cv2.bitwise_and(image, image, mask=mask)
cv2.imshow('hello', output)

# Convert the modified HSV image back to BGR
# result_image = cv2.cvtColor(output, cv2.COLOR_HSV2BGR)

# lower_bound = np.array([182, 151, 166])
lower_bound = np.array([140, 150, 150])
upper_bound = np.array([255, 255, 255])

# Create mask within BGR range
mask = cv2.inRange(output, lower_bound, upper_bound)

# Find contours in the mask
contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

# Draw contours on a copy of the original image
image_with_contours = image.copy()
cv2.drawContours(image_with_contours, contours, -1, (0, 255, 0), 2)

# Display the image with contours
cv2.imshow('image', image_with_contours)

while True:
    # Wait for a key event
    key = cv2.waitKey(0)

    # Break the loop if 'q' is pressed
    if key & 0xFF == ord('q'):
        break

# Close the window
cv2.destroyAllWindows()
