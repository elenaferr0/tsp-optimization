import argparse
import math
import random
import os
from typing import List, Tuple, Dict, Any

# Constants
MIN_SPACING = 0.8  # Minimum spacing between points (mm)
BOARD_MARGIN = 2.0  # Minimum distance from board edge (mm)
DEFAULT_DENSITY = 1  # Default point density (0-1)

class Pattern:
    def __init__(self, pattern_type, points):
        self.type = pattern_type
        self.points = points

def distance(p1: Tuple[float, float], p2: Tuple[float, float]) -> float:
    """Calculate Euclidean distance between two points."""
    return math.sqrt((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2)

def is_valid_point(point: Tuple[float, float], existing_points: List[Tuple[float, float]],
                   board_size: float, min_spacing: float = MIN_SPACING) -> bool:
    """Check if a point is valid (not too close to others or board edge)."""
    x, y = point

    # Check board margins
    if x < BOARD_MARGIN or y < BOARD_MARGIN or x > board_size - BOARD_MARGIN or y > board_size - BOARD_MARGIN:
        return False

    # Check spacing from other points
    for p in existing_points:
        if distance(point, p) < min_spacing:
            return False

    return True

def generate_parallelogram_pattern(board_size: float, existing_points: List[Tuple[float, float]],
                                   min_spacing: float = MIN_SPACING) -> Pattern:
    """Generate a parallelogram pattern."""
    max_attempts = 100

    for _ in range(max_attempts):
        # Random center position
        center_x = random.uniform(BOARD_MARGIN + 3, board_size - BOARD_MARGIN - 3)
        center_y = random.uniform(BOARD_MARGIN + 3, board_size - BOARD_MARGIN - 3)

        # Random width, height, skew and rotation
        width = random.uniform(2, min(6, board_size/5))
        height = random.uniform(2, min(6, board_size/5))
        skew = random.uniform(0.3, 1.0) * width  # Amount of horizontal offset for the parallelogram
        rotation = random.uniform(0, 2 * math.pi)

        # Generate the four points of the parallelogram
        points = []
        valid = True

        for i in range(4):
            if i == 0:
                dx, dy = -width/2, -height/2
            elif i == 1:
                dx, dy = width/2 - skew/2, -height/2
            elif i == 2:
                dx, dy = width/2 + skew/2, height/2
            else:
                dx, dy = -width/2 + skew/2, height/2

            # Apply rotation
            x = center_x + dx * math.cos(rotation) - dy * math.sin(rotation)
            y = center_y + dx * math.sin(rotation) + dy * math.cos(rotation)

            point = (x, y)
            if not is_valid_point(point, existing_points + points, board_size, min_spacing):
                valid = False
                break

            points.append(point)

        if valid:
            return Pattern("parallelogram", points)

    return Pattern("parallelogram", [])

def generate_line_pattern(board_size: float, existing_points: List[Tuple[float, float]],
                   min_spacing: float = MIN_SPACING) -> Pattern:
    num_points = random.randint(2, 4)
    max_attempts = 100

    # Try to find a valid starting point
    for _ in range(max_attempts):
        # Random position for the line
        center_x = random.uniform(BOARD_MARGIN + 2, board_size - BOARD_MARGIN - 2)
        center_y = random.uniform(BOARD_MARGIN + 2, board_size - BOARD_MARGIN - 2)

        angle = random.choice([0, math.pi/2])

        # Random length (scaled by board size) and point spacing variability
        line_length = random.uniform(2, min(8, board_size/3))

        # Decide if we want uniform or variable spacing
        uniform_spacing = random.choice([True, False])

        if uniform_spacing:
            # Uniform spacing between points
            spacing = line_length / (num_points - 1)
            offsets = [i * spacing - line_length / 2 for i in range(num_points)]
        else:
            # Variable spacing between points
            # Generate random positions along the line, then sort them
            raw_offsets = [random.uniform(-line_length/2, line_length/2) for _ in range(num_points)]
            offsets = sorted(raw_offsets)

        # Generate points along the line
        points = []
        valid = True

        for offset in offsets:
            x = center_x + offset * math.cos(angle)
            y = center_y + offset * math.sin(angle)

            point = (x, y)
            if not is_valid_point(point, existing_points + points, board_size, min_spacing):
                valid = False
                break

            points.append(point)

        if valid:
            return Pattern("line", points)

    # If we couldn't find a valid line after max attempts, return an empty pattern
    return Pattern("line", [])

def generate_triangle_pattern(board_size: float, existing_points: List[Tuple[float, float]],
                       min_spacing: float = MIN_SPACING) -> Pattern:
    """Generate a triangle pattern."""
    max_attempts = 100

    for _ in range(max_attempts):
        # Random center position
        center_x = random.uniform(BOARD_MARGIN + 3, board_size - BOARD_MARGIN - 3)
        center_y = random.uniform(BOARD_MARGIN + 3, board_size - BOARD_MARGIN - 3)

        # Random size and rotation
        size = random.uniform(1.5, min(7, board_size/5))
        rotation = random.uniform(0, 2 * math.pi)

        # Random triangle shape (by varying angles)
        # Create irregular triangles by randomizing angles
        irregular = random.choice([True, False])

        if irregular:
            # Create 3 random angles that sum to 2π
            angles = [random.random() for _ in range(3)]
            total = sum(angles)
            angles = [a * 2 * math.pi / total for a in angles]
            # Accumulate angles
            cumulative_angles = [0]
            for i in range(len(angles)):
                cumulative_angles.append(cumulative_angles[-1] + angles[i])
        else:
            # Regular triangle
            cumulative_angles = [0, 2*math.pi/3, 4*math.pi/3, 2*math.pi]

        # Generate the three points of the triangle
        points = []
        valid = True

        for i in range(3):
            angle = rotation + cumulative_angles[i]
            # Vary distances for irregular triangles
            radius = size if not irregular else size * random.uniform(0.7, 1.3)
            x = center_x + radius * math.cos(angle)
            y = center_y + radius * math.sin(angle)

            point = (x, y)
            if not is_valid_point(point, existing_points + points, board_size, min_spacing):
                valid = False
                break

            points.append(point)

        if valid:
            return Pattern("triangle", points)

    # If we couldn't find a valid triangle after max attempts, return an empty pattern
    return Pattern("triangle", [])

def generate_rectangle_pattern(board_size: float, existing_points: List[Tuple[float, float]],
                       min_spacing: float = MIN_SPACING) -> Pattern:
    max_attempts = 100

    for _ in range(max_attempts):
        # Random center position
        center_x = random.uniform(BOARD_MARGIN + 3, board_size - BOARD_MARGIN - 3)
        center_y = random.uniform(BOARD_MARGIN + 3, board_size - BOARD_MARGIN - 3)

        # Random width, height and rotation
        width = random.uniform(2, min(8, board_size/4))

        # Allow for both squares and rectangles with various aspect ratios
        if random.random() < 0.3:  # 30% chance to be square-ish
            height = width * random.uniform(0.9, 1.1)
        else:  # 70% chance to be rectangular with various aspect ratios
            height = width * random.uniform(0.3, 2.5)

        rotation = random.uniform(0, 2 * math.pi)

        # Generate the four points of the rectangle
        points = []
        valid = True

        for i in range(4):
            if i == 0:
                dx, dy = -width/2, -height/2
            elif i == 1:
                dx, dy = width/2, -height/2
            elif i == 2:
                dx, dy = width/2, height/2
            else:
                dx, dy = -width/2, height/2

            # Apply rotation
            x = center_x + dx * math.cos(rotation) - dy * math.sin(rotation)
            y = center_y + dx * math.sin(rotation) + dy * math.cos(rotation)

            point = (x, y)
            if not is_valid_point(point, existing_points + points, board_size, min_spacing):
                valid = False
                break

            points.append(point)

        if valid:
            return Pattern("rectangle", points)

    # If we couldn't find a valid rectangle after max attempts, return an empty pattern
    return Pattern("rectangle", [])

def generate_random_point(board_size: float, existing_points: List[Tuple[float, float]],
                    min_spacing: float = MIN_SPACING) -> Tuple[float, float]:
    max_attempts = 100

    for _ in range(max_attempts):
        x = random.uniform(BOARD_MARGIN, board_size - BOARD_MARGIN)
        y = random.uniform(BOARD_MARGIN, board_size - BOARD_MARGIN)

        point = (x, y)
        if is_valid_point(point, existing_points, board_size, min_spacing):
            return point

    # If we couldn't find a valid point after max attempts, return None
    return None

def generate_pcb_pattern(size: int, density: float = DEFAULT_DENSITY) -> List[Tuple[float, float]]:
    # Determine board size based on size parameter
    board_size = size * 1.5

    # Calculate number of patterns based on size and density
    num_patterns = max(1, int(size * density * 0.5))

    # Limit for random individual points
    max_random_points = size - num_patterns * 4  # Assuming average 4 points per pattern

    # Generate patterns
    all_points = []
    patterns = []

    # Add some lines
    for _ in range(random.randint(2, num_patterns)):
        pattern = generate_line_pattern(board_size, all_points)
        if pattern.points:
            patterns.append(pattern)
            all_points.extend(pattern.points)

    # Add some triangles
    for _ in range(random.randint(1, num_patterns)):
        pattern = generate_triangle_pattern(board_size, all_points)
        if pattern.points:
            patterns.append(pattern)
            all_points.extend(pattern.points)

    # Add some rectangles
    for _ in range(random.randint(1, num_patterns)):
        pattern = generate_rectangle_pattern(board_size, all_points)
        if pattern.points:
            patterns.append(pattern)
            all_points.extend(pattern.points)

    # Add some parallelograms
    for _ in range(random.randint(1, num_patterns)):
        pattern = generate_parallelogram_pattern(board_size, all_points)
        if pattern.points:
            patterns.append(pattern)
            all_points.extend(pattern.points)

    # Fill with random points up to size
    while len(all_points) < size and max_random_points > 0:
        point = generate_random_point(board_size, all_points)
        if point:
            all_points.append(point)
            max_random_points -= 1

    # If we still need more points, try to add them with reduced spacing
    reduced_spacing_factor = 0.7
    current_min_spacing = MIN_SPACING

    while len(all_points) < size:
        current_min_spacing *= reduced_spacing_factor
        point = generate_random_point(board_size, all_points, current_min_spacing)
        if point:
            all_points.append(point)
        else:
            # If still can't find a valid point, break to avoid infinite loop
            break

    # If we ended up with too many points, trim them
    if len(all_points) > size:
        all_points = all_points[:size]

    return all_points

def write_graph(coordinates: List[Tuple[float, float]], output: str) -> None:
    """Write the coordinates to a file in the required format."""
    n = len(coordinates)
    with open(output, 'w') as f:
        f.write(f"{n}\n")  # Number of nodes
        for i in range(n):
            x, y = coordinates[i]
            f.write(f"{i} {x:.6f} {y:.6f}\n")

def main():
    parser = argparse.ArgumentParser(description='PCB drilling pattern generator for TSP')
    parser.add_argument('-s', '--size', required=True, type=int, help='Number of drilling points')
    parser.add_argument('-d', '--density', type=float, default=DEFAULT_DENSITY,
                        help='Density of patterns (0.0-1.0), higher values create more structured patterns')
    parser.add_argument('-o', '--output', help='Output file path')
    args = parser.parse_args()

    if args.density < 0 or args.density > 1:
        parser.error("Density must be between 0.0 and 1.0")

    coordinates = generate_pcb_pattern(args.size, args.density)

    # If we couldn't generate enough points, inform the user
    if len(coordinates) < args.size:
        print(f"Warning: Could only generate {len(coordinates)} points due to spacing constraints")

    if args.output:
        output_file = args.output
    else:
        working_dir = os.path.dirname(os.path.abspath(__file__))
        output_file = f"{working_dir}/../samples/pcb_{args.size}.dat"

    # Ensure output directory exists
    os.makedirs(os.path.dirname(output_file), exist_ok=True)

    write_graph(coordinates, output_file)
    print(f"Generated PCB drilling pattern with {len(coordinates)} points and saved to {output_file}")

if __name__ == "__main__":
    main()
