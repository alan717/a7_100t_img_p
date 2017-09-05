 library ieee;
   use ieee.std_logic_1164.all;

 entity lfsr_bit is
   port (
     reset    : in  std_logic;
     clk      : in  std_logic; 
     rbit_out : out std_logic
   );
 end entity;

 architecture rtl of lfsr_bit is
   signal lfsr       : std_logic_vector (3 downto 0);
   signal feedback  : std_logic;

 begin

   -- option for LFSR size 4
   feedback <= not(lfsr(3) xor lfsr(2));  

   sr_pr : process (clk) 
   begin
     if (rising_edge(clk)) then
       if (reset = '1') then
         lfsr <= (others => '0');
       else
         lfsr <= lfsr(2 downto 0) & feedback;
       end if; 
     end if;
   end process sr_pr;

   rbit_out <= lfsr(3);
  
 end architecture;
