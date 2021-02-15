function sysCall_init()
    -- do some initialization here

    -- [[Objects handles]]
    robot_handle = sim.getObjectAssociatedWithScript(sim.handle_self)
    right_motor = sim.getObjectHandle("Right_wheel_joint")
    left_motor = sim.getObjectHandle("Left_wheel_joint")
    
    
    -- [[Distance Sensors]]
    right_distance_sensor = sim.getObjectHandle("Right_distance_sensor")
    left_distance_sensor = sim.getObjectHandle("Left_distance_sensor")
    max_dist = 1
    
    -- [[Infrared Sensor]]
    right_infrared_sensor = sim.getObjectHandle("RightSensor")
    left_infrared_sensor = sim.getObjectHandle("LeftSensor")
    line_right = 0
    line_left = 0

    -- init variables
    
    koe = 0
    cont_procurando = 0
    
end


-- estados procurando, atacando, fugindo
-- estilo o loop do arduino, fica atuando infinitamente at? dar algum comando se n?o me engano pra esse "sysCall parar de funcionar"
function sysCall_actuation()
    -- put your actuation code here
    estado = 'procurando'
    
    
    if (dist_right == nil and dist_left == nil) then -- primeiro valor do sensor de proximidade ? null, por isso esta aqui isso pra evitar erro
    elseif (dist_right == 1 and dist_left == 1 and koe == 0) then --viu nada aqui
        estado = 'procurando'
        cont_procurando = 0
        koe = 1
    end
    
    if estado == 'procurando' then
        if (line_left == 1 or line_right == 1) then
            estado = 'fugindo'
    
        elseif cont_procurando == 0 then
            motor_control(-1,6)
            cont_procurando = 1
        elseif cont_procurando == 1  then
            if (dist_left < 1 or dist_right < 1) then
                estado = 'atacando'
                cont_atacando = 0
            end
        else
            -- vai qualquer ideia aqui
        end
    end
    
    if estado == 'atacando' then
        if (line_left == 1 or line_right == 1) then
            estado = 'fugindo'
        elseif (dist_left < 1 and dist_right < 1) then
            motor_control(6,6)
        elseif (dist_left < 1 and dist_right == 1) then
            motor_control(-1,6)
        elseif (dist_right < 1 and dist_left == 1) then
            motor_control(6,-1)
        end    
    end
    
    if estado == 'fugindo' then
        if (line_left == 1 and line_right == 1) then
            motor_control(-1,-1)
        elseif (line_left == 1 and line_right == 0) then
            motor_control(3,-1)
        elseif (line_left == 0 and line_right == 1) then
            motor_control(-1,3)
        else
            estado = 'procurando'
            cont_procurando = 0
        end
    end
end


-- Esse sensing ? como se fosse um loop infinto, e a cada loop do syscall actuation, ele ? ativado
function sysCall_sensing()
    -- put your sensing code here
    dist_left,dist_right = getDistance(left_distance_sensor,right_distance_sensor,max_dist)
    --print(dist_right)
    line_right,line_left = getLineColour()
    --print(line_left,line_right)
end

function sysCall_cleanup()
    -- do some clean-up here
end

-- funcao para verificar oq os sensores lidars estao vendo, o max dist = 1 ? usado pra evitar o null aparecer na leitura, visto que quando o sensor n?o ve nada ele printa null
function getDistance(sensor_l,sensor_r)
    local detected, distance_r, distance_l
    detected,distance_r =sim.readProximitySensor(sensor_r)
    if detected<1 then
        distance_r = 1
    end
    detected, distance_l=sim.readProximitySensor(sensor_l)
    if detected<1 then
        distance_l = 1
    end
    return distance_l,distance_r

end


-- verifica??o dos infras
function getLineColour()
    value_left = sim.getVisionSensorImage(left_infrared_sensor)
    value_right = sim.getVisionSensorImage(right_infrared_sensor)
    return value_right[1],value_left[1]
end

function motor_control(v_motor_l,v_motor_r)
    
    sim.setJointTargetVelocity(left_motor, v_motor_l)
    sim.setJointTargetVelocity(right_motor, v_motor_r)
    
    
end