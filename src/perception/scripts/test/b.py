import cv2
import numpy as np

def has_barcode(image_path):
    # Load the image
    image = cv2.imread(image_path)
    
    # Convert the image to HSV color space
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    
    # Define the lower and upper bounds of the off-white color range (adjust these values)
    lower_off_white = np.array([0, 0, 130])
    upper_off_white = np.array([255, 255, 255])
    
    # Define the lower and upper bounds of the brown color range (adjust these values)
    lower_brown = np.array([10, 50, 50])
    upper_brown = np.array([30, 255, 255])
    
    # Create masks for off-white and brown colors
    off_white_mask = cv2.inRange(hsv, lower_off_white, upper_off_white)
    brown_mask = cv2.inRange(hsv, lower_brown, upper_brown)
    
    # Combine the masks to avoid brown areas
    combined_mask = cv2.bitwise_and(off_white_mask, cv2.bitwise_not(brown_mask))
    
    # Find contours in the combined mask
    contours, _ = cv2.findContours(combined_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    result_image = image.copy()
    
    # Check for large off-white sticker based on contour area
    for contour in contours:
        area = cv2.contourArea(contour)
        print(area)
        if area > 100:  # Adjust this threshold based on your needs
            cv2.drawContours(result_image,[contour], -1, (0,255,0), 2)
            #return "yes"
    
    #return "no"
    return result_image

# Provide the path to the image
image_path = "/home/umic/sumit2_ws/barcode.jpg"
result = has_barcode(image_path)
#print(result)
cv2.imwrite('contour.png', result)