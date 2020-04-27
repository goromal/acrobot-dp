log_prefix = "../build/ip_pm3/inverted_pendulum_tracking_p01eps";
num_trials = 400;

%%

close all

successes = logical(zeros(1, num_trials)); %#ok<LOGL>
theta0 = zeros(1,num_trials);
thetadot0 = zeros(1,num_trials);

for i=1:1:num_trials
    disp(i)
    logname = strcat(log_prefix, "_trial", num2str(i-1), ".log");
    figname = strcat(log_prefix, "_trial", num2str(i-1), ".fig");
    [t, x] = read_sim_log(logname);
    
    theta = wrap_0_to_2pi(x(1,:));
    thetadot = x(3,:);
    
    theta0(i) = theta(1);
    thetadot0(i) = thetadot(1);
    
    goal_idx = logical((abs(theta-pi) < 0.15) .* (abs(thetadot) < 0.5));
    
    if any(goal_idx)
        successes(i) = true;
    end
    
    figure()
    subplot(2,1,1)
    plot(t,theta,'k-',[t(1) t(end)],[pi pi],'b--',t(goal_idx),theta(goal_idx),'c*')
    ylabel('\theta')
    grid on
    subplot(2,1,2)
    plot(t,thetadot,'k-',[t(1) t(end)],[-2*pi -2*pi],'r--',[t(1) t(end)],[2*pi 2*pi],'r--',...
         t(goal_idx),thetadot(goal_idx),'c*')
    grid on
    xlabel('t (s)')
    ylabel('$\dot{\theta}$','interpreter','latex')
    
    savefig(gcf, figname)
    close all
    
end

figure()
hold on; grid on
for i = 1:1:num_trials
    if successes(i)
        plot(theta0(i),thetadot0(i),'c*','Linewidth',1.5)
    else
        plot(theta0(i),thetadot0(i),'r*','Linewidth',1.5)
    end
end
sp = 100 * sum(successes) / num_trials;
plot(0,0,'k+')
xlabel('\theta_0')
ylabel('$\dot{\theta}_0$','interpreter','latex')
hold off
title(['Success Percentage: ' num2str(sp) '%'])
xlim([0 2*pi])
ylim([-pi pi])