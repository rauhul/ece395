function simulate(port)
    close all;
    
    delete(instrfindall);
    s = serial(port);
    set(s, 'BaudRate', 115200);
    fopen(s);
    
    [xx,yy] = ndgrid(-4:4:4,-4:4:4);
    z = 0 * xx + 0 * yy;
    surface = surf(xx,yy,z);
    xlim([-10 10]);
    ylim([-10 10]);
    zlim([-10 10]);
    xlabel('X')
    ylabel('Y')
    zlabel('Z')

    pause(.1)
    
    while 1
        [success, quaternion] = readQuaternion(s);
        if success
            g = gravity(quaternion);

            z = (-g.x*xx - g.y*yy - 0)/g.z;

            set(surface, 'ZData', z);
            drawnow;
        end
    end
    
    close all;
    fclose(s);
    delete(s);
end

function [success, quaternion] = readQuaternion(s)
    [result, ~] = fscanf(s, '%ld %ld %ld %ld');
    if length(result) == 4 
        result = double(result);cd 
        result = result ./ norm(result);

        quaternion.w = result(1);
        quaternion.x = result(2);
        quaternion.y = result(3);
        quaternion.z = result(4);
        success = 1;
    else
        quaternion = 0;
        success = 0;
    end
end

function g = gravity(quaternion) 
    g.x = 2 * ( (quaternion.x * quaternion.z) - (quaternion.w * quaternion.y) );
    g.y = 2 * ( (quaternion.w * quaternion.x) + (quaternion.y * quaternion.z) );
    g.z = ( quaternion.w^2 - quaternion.x^2 - quaternion.y^2 + quaternion.z^2 );
end
