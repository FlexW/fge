skinned_mesh_comp = nil
stop_animation = true
animation_playing = false
movement = false

function update(delta_time)
   if (skinned_mesh_comp == nil) then
      skinned_mesh_comp = owner:find_component_by_type_name("Fge::SkinnedMeshComponent")
      skinned_mesh_comp = cast_skinned_mesh_component(skinned_mesh_comp)
   end

   window = app:get_graphic_manager():get_window()

   movement_speed = 40.0
   velocity = movement_speed * delta_time
   position = owner:get_position()
   forward = owner:get_forward()
   right = owner:get_right()

   -- Do movement
   if (window:get_key(Key.W) == KeyAction.Press) then

      position = position + forward * velocity
      owner:set_position(position)

      movement = true

   elseif (window:get_key(Key.S) == KeyAction.Press) then

      position = position - forward * velocity
      owner:set_position(position)

      movement = true

   elseif (window:get_key(Key.A) == KeyAction.Press) then

      position = position + right * velocity
      owner:set_position(position)

      movement = true

   elseif (window:get_key(Key.D) == KeyAction.Press) then

      position = position - right * velocity
      owner:set_position(position)

      movement = true

   else
      movement = false
      stop_animation = true
      animation_playing = false

   end

   if (movement) then
      if (not(animation_playing) and stop_animation) then
         animation_playing = true
         stop_animation = false
         skinned_mesh_comp:play_animation_endless("Animation 0")
      end
   end

   if (stop_animation) then
      skinned_mesh_comp:stop_current_animation()
   end

   -- Do rotation
   max_mouse_speed = 15.0
   max_angular_speed = 3.14159 * 8.0
   angular_speed = 0.0
   sensitivity = 0.2
   offset_x = window:get_mouse_offset_x()

   rotation = owner:get_rotation()

   if (offset_x ~= 0.0) then
      angular_speed = offset_x / max_mouse_speed
      angular_speed = angular_speed * max_angular_speed
   end

   angle = -1.0 * angular_speed * delta_time
   inc = angle_axis(angle, vec3.new(0.0, 1.0, 0.0))
   rotation = inc * rotation

   owner:set_rotation(rotation)

end
