def cross_2d(a, b):
    return (a[0] * b[1]) - (a[1] * b[0])

def get_intersection(ray_origin, ray_dir, edge_start, edge_end):    
    edge_dir = (edge_end[0] - edge_start[0], edge_end[1] - edge_start[1])
    cross_dir = cross_2d(ray_dir, edge_dir)
    
   
    if abs(cross_dir) < 0.0001:
        return False, None
        
    origin_diff = (edge_start[0] - ray_origin[0], edge_start[1] - ray_origin[1])
    
    
    t = cross_2d(origin_diff, edge_dir) / cross_dir
    u = cross_2d(origin_diff, ray_dir) / cross_dir
    
    
    if t > 0.0001 and 0.0 <= u <= 1.0:
        hit_point = (ray_origin[0] + ray_dir[0] * t, ray_origin[1] + ray_dir[1] * t)
        return True, hit_point
        
    return False, None

def split_polygon(poly):
    
    n = len(poly)
    if n < 3:
        return
        
    for k in range(n):
        index_k = k
        index_k1 = (k + 1) % n
        index_k2 = (k + 2) % n
        
        vk = poly[index_k]
        vk1 = poly[index_k1] 
        vk2 = poly[index_k2]
        
        edge1 = (vk1[0] - vk[0], vk1[1] - vk[1])
        edge2 = (vk2[0] - vk1[0], vk2[1] - vk1[1])
        
        
        if cross_2d(edge1, edge2) < -0.0001:
            print(f"--- Concavity Found at index {index_k1} ---")
            
            ray_origin = vk1
            ray_dir = (vk1[0] - vk[0], vk1[1] - vk[1])
            
            intersected_edge_index = -1
            hit_point = None
            
            
            for i in range(n):
                
                if i == index_k or i == index_k1:
                    continue
                    
                edge_start = poly[i]
                edge_end = poly[(i + 1) % n]
                
                hit, point = get_intersection(ray_origin, ray_dir, edge_start, edge_end)
                if hit:
                    intersected_edge_index = i
                    hit_point = point
                    break
                    
            if intersected_edge_index != -1:
                print(f"Ray hit edge starting at index {intersected_edge_index} at point ({hit_point[0]:.4f}, {hit_point[1]:.4f})")
                
                
                poly_a = [vk1]
                curr_idx = index_k2
                while curr_idx != (intersected_edge_index + 1) % n:
                    poly_a.append(poly[curr_idx])
                    curr_idx = (curr_idx + 1) % n
                poly_a.append(hit_point)
                
                
                poly_b = [hit_point]
                curr_idx = (intersected_edge_index + 1) % n
                while curr_idx != index_k2:
                    poly_b.append(poly[curr_idx])
                    curr_idx = (curr_idx + 1) % n
                    
                
                print("\nPolygon A Vertices:")
                for p in poly_a:
                    print(f"  ({p[0]:.4f}, {p[1]:.4f})")
                    
                print("\nPolygon B Vertices:")
                for p in poly_b:
                    print(f"  ({p[0]:.4f}, {p[1]:.4f})")
                    
                return 
                
    print("Polygon is fully convex!")

if __name__ == "__main__":
    
    star_perimeter = [
        ( 0.0,   0.5),   
        (-0.15,  0.15),  
        (-0.5,   0.0),   
        (-0.15, -0.15),  
        ( 0.0,  -0.5),   
        ( 0.15, -0.15),  
        ( 0.5,   0.0),   
        ( 0.15,  0.15)   
    ]
    
    print("Running Rotational Split on 4-Pointed Star...\n")
    split_polygon(star_perimeter)