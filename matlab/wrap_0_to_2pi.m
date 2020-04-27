function wrapped_data = wrap_0_to_2pi(data)

for i = 1:1:length(data)
    
   while data(i) >= 2*pi
       data(i) = data(i) - 2*pi;
   end
   while data(i) < 0
       data(i) = data(i) + 2*pi;
   end 
    
end

wrapped_data = data;

end