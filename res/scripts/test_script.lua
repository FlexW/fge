print("Hello from lua")

function update(delta_time)

   curr_time = get_current_time_millis()
   x = sin(0.001 * curr_time)
   z = cos(0.001 * curr_time)
   y = 0.0

   owner:set_position(vec3.new(x, y, z))
   owner:set_rotation(vec3.new(x, 0.0, z))
end
