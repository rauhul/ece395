function simulate_bike_stability
    clear; close all;
    %% Variables
    g = 9.8;                                                               % m / s / s

    l_c = 0.53;                                                            % m
    l_fly = l_c;                                                           % m

    m_fly = 5.44;                                                          % kg
    m_bike = 18.6;                                                         % kg
    m_total = m_fly + m_bike;                                              % kg

    global J_fly;
    J_fly = 0.04788*6;                                                     % kg * m * m
    J_bike = 5.5;                                                          % kg * m * m
    J_total = (J_bike) + (J_fly) + (m_fly * l_fly^2);                      % kg * m * m

    global c1 c2 c3;
    c1 = m_total * l_c^2;                                                  %
    c2 = J_total;                                                          %
    c3 = m_total * g * l_c;                                                %

    global err;
    err = 0;                                                               %

    global target_bike_theta target_motor_theta_dot;
    target_bike_theta = 0;                                                 % rad
    target_motor_theta_dot = 0;                                            % rad / s

    global stall_torque free_speed;
    ratio = 60 / 12 * 60 / 18;
    stall_torque = 2*2.43*ratio;                                              % N * m
    free_speed = 556/ratio;                                                   % rad / s

    global max_torque percent_max_torque;                                  %
    max_torque = 0;
    percent_max_torque = 0;

    %% C-Style Main Function
    main();
    function main

        t_span = [0 10];

        % bike_theta, bike_theta_dot, motor_theta_dot
        intial_angle = 2;
        intial = [intial_angle*pi/180 0.1 0];
        [t, state] = ode45(@bike_falling_ode, t_span, intial);

        % convert bike_theta to degrees
        state(:, 1) = state(:,1) * (180/pi);

        % plot
        figure;
        subplot(3, 1, 1);
        hold;
        plot(t, state(:,1))
        plot(t, state(:,3))
        hold;
        title('bike theta vs time, motor theta dot vs time');

        subplot(3, 1, 2);
        plot(t, state(:,2))
        title('bike theta dot vs time');
        
        subplot(3, 1, 3);
        plot(state(:,1), state(:,2))
        title('bike theta dot vs bike theta');

        max_torque
        percent_max_torque
    end


    %% Bike Falling
    function delta = bike_falling_ode( ~, state )

        % unpack state
        bike_theta      = state(1);
        bike_theta_dot  = state(2);
        motor_theta_dot = state(3);

        % get feedback from PID controller
        torque = bike_falling_PID(bike_theta, bike_theta_dot, motor_theta_dot);

        % solve for bike_theta_dot_dot
        bike_theta_dot_dot_numerator = torque - c1*bike_theta_dot^2*sin(bike_theta)*cos(bike_theta) + c3*sin(bike_theta);
        bike_theta_dot_dot_denominator = c1*sin(bike_theta)^2 + c2;

        % solve for motor_theta_dot_dot
        motor_theta_dot_dot = torque/J_fly;

        % pack delta
        delta = [
            bike_theta_dot;
            bike_theta_dot_dot_numerator/bike_theta_dot_dot_denominator;
            motor_theta_dot_dot
        ];
    end

    %% PID Loop
    function torque = bike_falling_PID(bike_theta, bike_theta_dot, motor_theta_dot)
        current_err = target_bike_theta - bike_theta;                      % P
        err = current_err + err;                                           % I
        diff = bike_theta_dot;                                             % D

        % PID constants
        kPID = [200, 0, -30];

        % compute torque
        torque = (kPID(1) * current_err) + (kPID(2) * err) + (kPID(3) * diff);

        torque_limit = stall_torque * (1 - abs(motor_theta_dot)/free_speed);

        if abs(torque)/torque_limit * 100 > percent_max_torque
            percent_max_torque = abs(torque)/torque_limit * 100;
        end

        % limit torque
        if torque > torque_limit
            torque = torque_limit;
        elseif torque < -torque_limit
            torque = -torque_limit;
        end

        % log maximum torque value required by the system
        if abs(torque) > max_torque
            max_torque = abs(torque);
        end


    end

end
