library ieee;
  use ieee.std_logic_1164.all;
  use work.lfsr_pkg.all;
  
entity lfsr1 is
  port (
    reset  : in  std_logic;
    clk    : in  std_logic; 
    en     : in  std_logic; 
    count  : out std_logic_vector (LFSR_W-1 downto 0)    -- lfsr output
  );
end entity;

architecture rtl of lfsr1 is
    signal count_i    : std_logic_vector (LFSR_W-1 downto 0);
    signal feedback   : std_logic;

begin
  -- option for LFSR size 11
  feedback <= not(count_i(LFSR_W-1) xor count_i(LFSR_W-3));  

  sr_pr : process (clk) 
    begin
      if (rising_edge(clk)) then
        if (reset = '1') then
          count_i <= (others=>'0');
        elsif (en = '1') then
          count_i <= count_i(LFSR_W-2 downto 0) & feedback;
        end if;  
      end if;
    end process sr_pr;
  count <= count_i;
  
end architecture;
