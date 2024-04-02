import math

def compute_points(slope, x1, y1, d):
    # Calculate y-intercept (b) using the given point (x1, y1)
    b = y1 - slope * x1
    
    # Calculate new points at a distance 'd' from (x1, y1)
    delta_x = d / math.sqrt(1 + slope**2)
    x2 = x1 + delta_x
    x3 = x1 - delta_x
    y2 = y1 + slope * delta_x
    y3 = y1 - slope * delta_x
    
    return [(x2, y2), (x3, y3)]

# Example usage
# slope = 1  # Example slope
# x1 = -0.707107     # Example x-coordinate of the given point
# y1 = 0     # Example y-coordinate of the given point
# d = 1.5      # Example distance

# slope = -1
# x1 = 0.35355317177982115
# y1 = 1.0606601717798212
# d = 1.2

slope = 1
x1 = 1.202081309203678
y1 = 0.21213203435596428
d = 1.5

points = compute_points(slope, x1, y1, d)
print("Points at a distance {} from ({}, {}):".format(d, x1, y1))
for point in points:
    print(point)
