logfile = '../build/ab_pm5/acrobot_inputs_p01eps.log';

u = read_u_ab_log(logfile);

u_slice1 = rot90(squeeze(u(:,:,1,1)),2);
u_slice2 = rot90(squeeze(u(:,:,15,2)),2);
u_slice3 = rot90(squeeze(u(:,:,50,100)),2);

figure('Position',[500 500 1500 500])
subplot(1,3,1)
image(u_slice1,'CDataMapping','scaled')
xticks([1 36])
xticklabels({'0','2\pi'})
xlabel('$\dot{\theta}$','Interpreter','latex')
yticks([1 18])
yticklabels({'\pi','0'})
ylabel('\theta')
pbaspect([2 1 1])
colorbar
subplot(1,3,2)
image(u_slice2,'CDataMapping','scaled')
xticks([1 36])
xticklabels({'0','2\pi'})
xlabel('$\dot{\theta}$','Interpreter','latex')
yticks([1 18])
yticklabels({'\pi','0'})
ylabel('\theta')
pbaspect([2 1 1])
colorbar
subplot(1,3,3)
image(u_slice3,'CDataMapping','scaled')
xticks([1 36])
xticklabels({'0','2\pi'})
xlabel('$\dot{\theta}$','Interpreter','latex')
yticks([1 18])
yticklabels({'\pi','0'})
ylabel('\theta')
pbaspect([2 1 1])
colorbar