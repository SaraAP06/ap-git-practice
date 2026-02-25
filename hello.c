import cv2
import numpy as np

# خواندن تصویر
image = cv2.imread("image.jpg")   # نام فایل تصویر را اینجا بگذار
output = image.copy()

# تبدیل به HSV
hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

# محدوده رنگ قرمز در HSV
lower_red1 = np.array([0, 120, 70])
upper_red1 = np.array([10, 255, 255])

lower_red2 = np.array([170, 120, 70])
upper_red2 = np.array([180, 255, 255])

# ساخت ماسک برای رنگ قرمز
mask1 = cv2.inRange(hsv, lower_red1, upper_red1)
mask2 = cv2.inRange(hsv, lower_red2, upper_red2)
mask = mask1 + mask2

# حذف نویز
kernel = np.ones((5, 5), np.uint8)
mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)

# پیدا کردن کانتورها
contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

for cnt in contours:
    area = cv2.contourArea(cnt)
    
    # فیلتر کردن نواحی کوچک
    if area > 500:
        (x, y), radius = cv2.minEnclosingCircle(cnt)
        center = (int(x), int(y))
        radius = int(radius)
        
        # رسم دایره دور توپ قرمز
        cv2.circle(output, center, radius, (0, 255, 0), 3)

# نمایش نتیجه
cv2.imshow("Red Balls Detection", output)
cv2.waitKey(0)
cv2.destroyAllWindows()
