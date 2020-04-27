logfile1 = '../build/ip_pm5/inverted_pendulum_inputs_p01eps.log';
% logfile2 = '../build/inverted_pendulum_inputs_1eps.log';
% logfile3 = '../build/inverted_pendulum_inputs_p01eps.log';

%% 

u1 = read_u_log(logfile1);
u_chop1 = rot90(squeeze(u1(:,1,:,1)),2);

% u2 = read_u_log(logfile2);
% u_chop2 = rot90(squeeze(u2(:,1,:,1)),2);
% 
% u3 = read_u_log(logfile3);
% u_chop3 = rot90(squeeze(u3(:,1,:,1)),2);

% figure('Position',[500 500 1000 1200])
figure('Position',[500 500 1000 400])
% subplot(3,1,1)
image(u_chop1,'CDataMapping','scaled')
xticks([1 72])
xticklabels({'-2\pi','2\pi'})
xlabel('$\dot{\theta}$','Interpreter','latex')
yticks([1 18])
yticklabels({'\pi','0'})
ylabel('\theta')
pbaspect([4 1 1])
% title('\epsilon = 10.0')
colorbar
% subplot(3,1,2)
% image(u_chop2,'CDataMapping','scaled')
% xticks([1 72])
% xticklabels({'-2\pi','2\pi'})
% xlabel('$\dot{\theta}$','Interpreter','latex')
% yticks([1 18])
% yticklabels({'\pi','0'})
% ylabel('\theta')
% pbaspect([4 1 1])
% title('\epsilon = 1.0')
% subplot(3,1,3)
% image(u_chop3,'CDataMapping','scaled')
% xticks([1 72])
% xticklabels({'-2\pi','2\pi'})
% xlabel('$\dot{\theta}$','Interpreter','latex')
% yticks([1 18])
% yticklabels({'\pi','0'})
% ylabel('\theta')
% pbaspect([4 1 1])
% title('\epsilon = 0.01')
