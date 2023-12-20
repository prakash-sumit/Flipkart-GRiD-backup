import pyzed.sl as sl

def main():
    # Create a ZED Camera object
    zed = sl.Camera()

    # Set configuration parameters
    init_params = sl.InitParameters()
    init_params.depth_mode = sl.DEPTH_MODE.PERFORMANCE  # Adjust as needed

    # Open the camera
    err = zed.open(init_params)
    if err != sl.ERROR_CODE.SUCCESS:
        print(f"Error {err}: Unable to open ZED Camera")
        return

    # Enable spatial mapping
    mapping_params = sl.SpatialMappingParameters()
    mapping_params.resolution_meter = 0.1  # Voxel size in meters, adjust as needed
    mapping_params.range_meter = 10.0  # Maximum range of the mapping, adjust as needed

    err = zed.enable_spatial_mapping(mapping_params)
    if err != sl.ERROR_CODE.SUCCESS:
        print(f"Error {err}: Unable to enable spatial mapping")
        zed.close()
        return

    # Main loop
    while True:
        if zed.grab() == sl.ERROR_CODE.SUCCESS:
            # Retrieve the left image and depth map
            left_image = sl.Mat()
            depth_map = sl.Mat()
            zed.retrieve_image(left_image, sl.VIEW.LEFT)
            zed.retrieve_measure(depth_map, sl.MEASURE.DEPTH)

            # Update the spatial map
            zed.update_spatial_map()

            # Display or process the images as needed

    # Retrieve the spatial map
    spatial_map = sl.Mesh()
    zed.extract_spatial_map(spatial_map)

    # Save the spatial map to a file (e.g., in .ply format)
    spatial_map.save("spatial_map.ply")

    # Disable spatial mapping and close the camera
    zed.disable_spatial_mapping()
    zed.close()

if __name__ == "__main__":
    main()
