import argparse
import math
import random
import os
from typing import List, Tuple, Dict, Any

MIN_SPACING = 1.5  # Increased minimum spacing between points (mm)
BOARD_MARGIN = 3.0  # Increased minimum distance from board edge (mm)
PATTERN_SEPARATION = 4.0  # Minimum distance between different patterns
DEFAULT_DENSITY = 0.4  # Default point density (0-1)

class Pattern:
    def __init__(self, pattern_type, points):
        self.type = pattern_type
        self.points = points

def distance(p1: Tuple[float, float], p2: Tuple[float, float]) -> float:
    return math.sqrt((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2)

def is_valid_point(point: Tuple[float, float], existing_points: List[Tuple[float, float]],
                   board_size: float, min_spacing: float = MIN_SPACING) -> bool:
    """Check if a point is valid (not too close to others or board edge)."""
    x, y = point

    if x < BOARD_MARGIN or y < BOARD_MARGIN or x > board_size - BOARD_MARGIN or y > board_size - BOARD_MARGIN:
        return False

    # Check spacing from other points
    for p in existing_points:
        if distance(point, p) < min_spacing:
            return False

    return True

def is_pattern_valid(pattern_points: List[Tuple[float, float]], existing_patterns: List[Pattern],
                     board_size: float) -> bool:
    """Check if a pattern is far enough from existing patterns."""
    if not existing_patterns:
        return True

    for pattern_point in pattern_points:
        for existing_pattern in existing_patterns:
            for existing_point in existing_pattern.points:
                if distance(pattern_point, existing_point) < PATTERN_SEPARATION:
                    return False
    return True

def generate_parallelogram_pattern(board_size: float, existing_patterns: List[Pattern],
                                   min_spacing: float = MIN_SPACING) -> Pattern:
    max_attempts = 200

    for _ in range(max_attempts):
        # Random center position with more margin
        center_x = random.uniform(BOARD_MARGIN + 5, board_size - BOARD_MARGIN - 5)
        center_y = random.uniform(BOARD_MARGIN + 5, board_size - BOARD_MARGIN - 5)

        # Increased sizes for more prominent patterns
        width = random.uniform(3, min(10, board_size/4))
        height = random.uniform(3, min(10, board_size/4))
        skew = random.uniform(0.4, 1.2) * width
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
            if not is_valid_point(point, points, board_size, min_spacing):
                valid = False
                break

            points.append(point)

        if valid and is_pattern_valid(points, existing_patterns, board_size):
            return Pattern("parallelogram", points)

    return Pattern("parallelogram", [])

def generate_line_pattern(board_size: float, existing_patterns: List[Pattern],
                   min_spacing: float = MIN_SPACING) -> Pattern:
    num_points = random.randint(3, 5)  # Increased number of points
    max_attempts = 200

    # Try to find a valid starting point
    for _ in range(max_attempts):
        center_x = random.uniform(BOARD_MARGIN + 4, board_size - BOARD_MARGIN - 4)
        center_y = random.uniform(BOARD_MARGIN + 4, board_size - BOARD_MARGIN - 4)

        angle = random.choice([0, math.pi/2, math.pi/4, 3*math.pi/4])

        line_length = random.uniform(4, min(12, board_size/2.5))

        uniform_spacing = random.choice([True, False])

        if uniform_spacing:
            # Uniform spacing between points with minimum distance
            spacing = max(min_spacing * 1.2, line_length / (num_points - 1))
            actual_length = spacing * (num_points - 1)
            offsets = [i * spacing - actual_length / 2 for i in range(num_points)]
        else:
            # Variable spacing between points, ensuring minimum distance
            positions = []
            for i in range(num_points):
                if i == 0:
                    positions.append(-line_length/2)
                elif i == num_points - 1:
                    positions.append(line_length/2)
                else:
                    # Ensure minimum spacing
                    min_pos = positions[-1] + min_spacing * 1.2
                    max_pos = line_length/2 - (num_points - i - 1) * min_spacing * 1.2
                    if min_pos < max_pos:
                        positions.append(random.uniform(min_pos, max_pos))
                    else:
                        positions.append(min_pos)
            offsets = positions

        # Generate points along the line
        points = []
        valid = True

        for offset in offsets:
            x = center_x + offset * math.cos(angle)
            y = center_y + offset * math.sin(angle)

            point = (x, y)
            if not is_valid_point(point, points, board_size, min_spacing):
                valid = False
                break

            points.append(point)

        if valid and is_pattern_valid(points, existing_patterns, board_size):
            return Pattern("line", points)

    return Pattern("line", [])

def generate_triangle_pattern(board_size: float, existing_patterns: List[Pattern],
                       min_spacing: float = MIN_SPACING) -> Pattern:
    max_attempts = 200

    for _ in range(max_attempts):
        center_x = random.uniform(BOARD_MARGIN + 5, board_size - BOARD_MARGIN - 5)
        center_y = random.uniform(BOARD_MARGIN + 5, board_size - BOARD_MARGIN - 5)

        # Increased size for more prominent triangles
        size = random.uniform(3, min(12, board_size/3))
        rotation = random.uniform(0, 2 * math.pi)

        # Random triangle shape (by varying angles)
        irregular = random.choice([True, False])

        if irregular:
            # Create 3 random angles that sum to 2π, but avoid very sharp angles
            angles = [random.uniform(0.5, 2.0) for _ in range(3)]
            total = sum(angles)
            angles = [a * 2 * math.pi / total for a in angles]
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
            # Vary distances for irregular triangles, but ensure minimum spacing
            if irregular:
                radius = size * random.uniform(0.8, 1.2)
            else:
                radius = size

            x = center_x + radius * math.cos(angle)
            y = center_y + radius * math.sin(angle)

            point = (x, y)
            if not is_valid_point(point, points, board_size, min_spacing):
                valid = False
                break

            points.append(point)

        # ensure triangle points are far enough apart
        if valid:
            for i in range(len(points)):
                for j in range(i+1, len(points)):
                    if distance(points[i], points[j]) < min_spacing * 1.5:
                        valid = False
                        break
                if not valid:
                    break

        if valid and is_pattern_valid(points, existing_patterns, board_size):
            return Pattern("triangle", points)

    return Pattern("triangle", [])

def generate_rectangle_pattern(board_size: float, existing_patterns: List[Pattern],
                       min_spacing: float = MIN_SPACING) -> Pattern:
    max_attempts = 200

    for _ in range(max_attempts):
        center_x = random.uniform(BOARD_MARGIN + 5, board_size - BOARD_MARGIN - 5)
        center_y = random.uniform(BOARD_MARGIN + 5, board_size - BOARD_MARGIN - 5)

        # Increased width and height for more prominent rectangles
        width = random.uniform(4, min(12, board_size/3))

        # Allow for both squares and rectangles with various aspect ratios
        if random.random() < 0.3:  # square-ish
            height = width * random.uniform(0.9, 1.1)
        else:  # rectangular
            height = width * random.uniform(0.4, 2.0)

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
            if not is_valid_point(point, points, board_size, min_spacing):
                valid = False
                break

            points.append(point)

        if valid and is_pattern_valid(points, existing_patterns, board_size):
            return Pattern("rectangle", points)

    return Pattern("rectangle", [])

def generate_random_point(board_size: float, existing_points: List[Tuple[float, float]],
                    min_spacing: float = MIN_SPACING) -> Tuple[float, float]:
    max_attempts = 200

    for _ in range(max_attempts):
        x = random.uniform(BOARD_MARGIN, board_size - BOARD_MARGIN)
        y = random.uniform(BOARD_MARGIN, board_size - BOARD_MARGIN)

        point = (x, y)
        if is_valid_point(point, existing_points, board_size, min_spacing):
            return point

    return None

def generate_pcb_pattern(size: int, density: float = DEFAULT_DENSITY) -> List[Tuple[float, float]]:
    # Increased board size to accommodate larger patterns and spacing
    board_size = size * 2.5

    # Adjust number of patterns based on size and density
    num_patterns = max(1, int(size * density * 0.3))  # Fewer patterns for better spacing

    # Generate patterns with better separation
    all_points = []
    patterns = []

    # Generate different types of patterns
    pattern_generators = [
        generate_line_pattern,
        generate_triangle_pattern,
        generate_rectangle_pattern,
        generate_parallelogram_pattern
    ]

    # Generate patterns of each type
    for generator in pattern_generators:
        for _ in range(random.randint(1, max(1, num_patterns // 2))):
            pattern = generator(board_size, patterns)
            if pattern.points:
                patterns.append(pattern)
                all_points.extend(pattern.points)

    # Fill with random points up to size, but with better spacing
    attempts = 0
    max_fill_attempts = size * 10

    while len(all_points) < size and attempts < max_fill_attempts:
        point = generate_random_point(board_size, all_points, MIN_SPACING * 1.2)
        if point:
            all_points.append(point)
        attempts += 1

    # If we still need more points, gradually reduce spacing requirements
    spacing_reductions = [0.9, 0.8, 0.7, 0.6]

    for reduction in spacing_reductions:
        if len(all_points) >= size:
            break

        current_spacing = MIN_SPACING * reduction
        attempts = 0

        while len(all_points) < size and attempts < size * 5:
            point = generate_random_point(board_size, all_points, current_spacing)
            if point:
                all_points.append(point)
            attempts += 1

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
    parser = argparse.ArgumentParser(description='PCB drilling pattern generator for TSP with enhanced pattern visibility')
    parser.add_argument('-s', '--size', required=True, type=int, help='Number of drilling points')
    parser.add_argument('-d', '--density', type=float, default=DEFAULT_DENSITY,
                        help='Density of patterns (0.0-1.0), higher values create more structured patterns')
    parser.add_argument('-o', '--output-dir', required=True, help='Output directory for the generated instance')
    args = parser.parse_args()

    if args.density < 0 or args.density > 1:
        parser.error("Density must be between 0.0 and 1.0")

    coordinates = generate_pcb_pattern(args.size, args.density)

    if len(coordinates) < args.size:
        print(f"Warning: Could only generate {len(coordinates)} points due to spacing constraints")

    output_file = os.path.join(args.output_dir, f'random_{args.size}.dat')

    # Ensure output directory exists
    os.makedirs(os.path.dirname(output_file), exist_ok=True)

    write_graph(coordinates, output_file)
    print(f"Generated instance with {len(coordinates)} points to {output_file}")

if __name__ == "__main__":
    main()
