function simulate_bike_stability
    clear; close all;
    %% Variables
    g = 9.8;                                                               % m / s / s
    
    l_c = 0.53;                                                            % m
    l_fly = l_c;                                                           % m
    
    m_fly = 5.44;                                                          % kg
    m_bike = 18.6;                                                         % kg
    m_total = m_fly + m_bike;                                              % kg
    
    J_fly = 0.5;                                                           % kg * m * m
    J_bike = 5.5;                                                          % kg * m * m
    J_total = (J_bike) + (J_fly) + (m_fly * l_fly^2);                      % kg * m * m
    
    %% Global Variables
    global c1 c2 c3;
    c1 = m_total * l_c^2;                                                  %
    c2 = J_total;                                                          %
    c3 = m_total * g * l_c;                                                %
    
    global err theta_prev dt;
    err = 0;                                                               %
    theta_prev = 0;                                                        % rad / s
    dt = 2e-2;                                                             % s
    
    global target_theta torque_limit;
    target_theta = 0;                                                      % rad
    torque_limit = 15;                                                     % N * m
    
    
    
    
    %% C-Style Main Function
    main();
    function main 
    
        
        t_span = 0:dt:10;
        intial = [10 1];
        [t, state] = ode45(@bike_falling_ode, t_span, intial);

        % convert theta to degrees
        state(:, 1) = state(:,1) .* (180/pi);

        % plot
        figure;
        plotyy(t, state(:,1), t, state(:,2))
        title('Theta, Theta_Dot vs Time');
        
        figure;
        plot(state(:,1), state(:,2))
        title('Theta_Dot vs Theta');
    end





    %% Bike Falling
    function delta = bike_falling_ode( t, state )
        
        % unpack state
        theta     = state(1);
        theta_dot = state(2);
        
        % get feedback from PID controller
        torque = bike_falling_PID(theta, theta_dot);
        
        % solve for theta_dot_dot
        theta_dot_dot_numerator = torque - c1*theta_dot^2*sin(theta)*cos(theta) + c3*sin(theta);
        theta_dot_dot_denominator = c1*sin(theta)^2 + c2;
        
        % pack delta
        delta = [
            theta_dot; 
            theta_dot_dot_numerator/theta_dot_dot_denominator
        ];
    end

    %% PID Loop
    function torque = bike_falling_PID(theta, theta_dot)
        current_err = target_theta - theta;                                % P
        
        err = current_err + err;                                           % I
        
        diff = (theta - theta_prev) / dt;                                  % D
        theta_prev = theta;
    
        % PID constants
        kPID = [0, 0, 0];

        % compute torque
        torque = (kPID(1) * current_err) + (kPID(2) * err) + (kPID(3) * diff);
        
        % limit torque
        if torque > torque_limit
            torque = torque_limit;
        elseif torque < -torque_limit 
            torque = -torque_limit;
        end
    end
    
    %% Bike Properties
    

end
