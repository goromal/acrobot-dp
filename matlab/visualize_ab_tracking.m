log_prefix = "../build/ab_pm5/acrobot_tracking_p01eps";
num_trials = 400;
h = 2;
l1 = 1;
l2 = 2;

%%

close all

successes = logical(zeros(1, num_trials)); %#ok<LOGL>
theta1_0 = zeros(1,num_trials);
theta2_0 = zeros(1,num_trials);

for i=1:1:num_trials
    disp(i)
    logname = strcat(log_prefix, "_trial", num2str(i-1), ".log");
    figname = strcat(log_prefix, "_trial", num2str(i-1), ".fig");
    [t, x] = read_sim_log(logname);
    
    theta1 = wrap_0_to_2pi(x(1,:));
    theta2 = wrap_0_to_2pi(x(2,:));
    theta1dot = x(3,:);
    theta2dot = x(4,:);
    
    theta1_0(i) = theta1(1);
    theta2_0(i) = theta2(1);
    
    goal_idx = -l1*cos(theta1)-l2*cos(theta1+theta2) >= h;
    
    if any(goal_idx)
        successes(i) = true;
    end
    
    figure()
    subplot(4,1,1)
    plot(t,theta1,'k-',t(goal_idx),theta1(goal_idx),'c*')
    ylabel('\theta_1')
    grid on
    subplot(4,1,2)
    plot(t,theta2,'k-',t(goal_idx),theta2(goal_idx),'c*')
    ylabel('\theta_2')
    grid on
    subplot(4,1,3)
    plot(t,theta1dot,'k-',t(goal_idx),theta1dot(goal_idx),'c*',[t(1) t(end)],[-4*pi -4*pi],'r--',[t(1) t(end)],[4*pi 4*pi],'r--')
    ylabel('$\dot{\theta}_1$','interpreter','latex')
    grid on
    subplot(4,1,4)
    plot(t,theta2dot,'k-',[t(1) t(end)],[-4*pi -4*pi],'r--',[t(1) t(end)],[4*pi 4*pi],'r--',...
         t(goal_idx),theta2dot(goal_idx),'c*')
    grid on
    xlabel('t (s)')
    ylabel('$\dot{\theta}_2$','interpreter','latex')
    
    savefig(gcf, figname)
    close all
    
end

figure()
hold on; grid on
for i = 1:1:num_trials
    if successes(i)
        plot(theta1_0(i),theta2_0(i),'c*','Linewidth',1.5)
    else
        plot(theta1_0(i),theta2_0(i),'r*','Linewidth',1.5)
    end
end
sp = 100 * sum(successes) / num_trials;
plot(0,0,'k+')
xlabel('\theta_1')
ylabel('\theta_2')
hold off
title(['Success Percentage: ' num2str(sp) '%'])
xlim([0 2*pi])
ylim([0 2*pi])