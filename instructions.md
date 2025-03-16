# 需要实现的指令

本表格为写昏了头的助教为了知道自己要干什么从实验文档中粘过来的，你也可以参考。

|       LA32R 中指令    |     RV32I 中指令         |            指令功能                                  |           说明           |
| :------------------: | :----------------------: | :----------------------------:                      | :----------------------: | 
|   add.w rd, rj, rk   |     add rd, rs1, rs2     | GR[rd] = GR[rj] + GR[rk] </br> x[rd] = x[rs1] + x[rs2]                   |         整数加法          | 
|  addi.w rd, rj, imm  |     addi rd, rs1, imm    | GR[rd] = GR[rj] + sext(imm) </br> x[rd] = x[rs1] + sext(imm) |         整数加法          | 
|   sub.w rd, rj, rk   |     sub rd, rs1, rs2     | GR[rd] = GR[rj] - GR[rk] </br> x[rd] = x[rs1] - x[rs2]                  |         整数减法          | 
|    slt rd, rj, rk    |     slt rd, rs1, rs2     | GR[rd] = (GR[rj] < GR[rk]) </br> x[rd] = (x[rs1] < x[rs2])            |      有符号整数比较       | 
|   sltu rd, rj, rk    |    sltu rd, rs1, rs2     | GR[rd] = (GR[rj] <u GR[rk]) </br> x[rd] = (x[rs1] <u x[rs2])              |      无符号整数比较       | 
|    and rd, rj, rk    |     and rd, rs1, rs2     | GR[rd] = GR[rj] & GR[rk] </br> x[rd] = x[rs1] & x[rs2]          |          按位与          | 
|    or rd, rj, rk     |     or rd, rs1, rs2      | GR[rd] = GR[rj] \| GR[rk] </br> x[rd] = x[rs1] \| 𝑥[rs2] |          按位或          | 
|    xor rd, rj, rk    |     xor rd, rs1, rs2     | GR[rd] = GR[rj] ^ GR[rk] </br> x[rd] = x[rs1] ^ 𝑥[rs2]                    |         按位异或         | 
|   sll.w rd, rj, rk   |     sll rd, rs1, rs2     | GR[rd] = GR[rj] << GR[rk][4:0]  </br> x[rd] = x[rs1] << x[rs2][4:0]          |         逻辑左移         | 
|   srl.w rd, rj, rk   |     srl rd, rs1, rs2     | GR[rd]= GR[rj] >> GR[rk][4:0]  </br> x[rd] = x[rs1] >> x[rs2][4:0]  |         逻辑右移         | 
|   sra.w rd, rj, rk   |     sra rd, rs1, rs2     | GR[rd] = GR[rj] >>> GR[rk][4:0]  </br> x[rd] = x[rs1] >>> x[rs2][4:0]         |         算术右移         | 
|  slli.w rd, rj, imm  |   slli rd, rs1, shamt    | GR[rd] = GR[rj] << imm </br> x[rd] = x[rs1] << shamt|         逻辑左移         | 
|  srli.w rd, rj, imm  |   srli rd, rs1, shamt    | GR[rd] = GR[rj] >> imm </br> x[rd] = x[rs1] >> shamt        |         逻辑右移         | 
|  srai.w rd, rj, imm  |   srai rd, rs1, shamt    | GR[rd] = GR[rj] >>> imm </br>  x[rd] = x[rs1] >>> shamt      |         算术右移         | 
|   slti rd, rj, imm   |   slti rd, rs1, imm    | GR[rd] = (GR[rj] < sext(imm)) </br> x[rd] = (x[rs1] < sext(imm))  |      有符号整数比较      | 
|  sltui rd, rj, imm   |   sltiu rd, rs1, imm   | GR[rd] = (GR[rj] <u sext(imm)) </br> x[rd] = (x[rs1] <u sext(imm))        |      无符号整数比较      | 
|   andi rd, rj, imm   |   andi rd, rs1, imm    | GR[rd] = GR[rj] & zext(imm)  </br> x[rd] = x[rs1] & sext(imm) |          按位与          | 
|   ori rd, rj, imm    |    ori rd, rs1, imm    | GR[rd] = GR[rj] \| zext(imm)   </br> x[rd] = x[rs1] \| sext(imm)       |          按位或          | 
|   xori rd, rj, imm   |   xori rd, rs1, imm    | GR[rd] = GR[rj] ^ zext(imm)   </br> x[rd] = x[rs1] ^  sext(imm)      |         按位异或         | 
|  lu12i.w rd, imm     |       lui rd, imm        | GR[rd] = imm << 12 </br> x[rd] = sext(imm << 12)         |     加载高20位立即数     | 
|  pcaddu12i rd, imm   |      auipc rd, imm       | GR[rd] = pc + sext(imm << 12) </br> x[rd] = pc + sext(imm[31:12] << 12) | 加载加上 PC 的高 20 位立即数 |
|   ld.w rd, rj, imm   |     lw rd, offset(rs1)      |  GR[rd] = Mem[GR[rj] + imm] </br> x[rd] = sext(M[x[rs1] + sext(offset)])      |          加载字          | 
|   ld.h rd, rj, imm   |     lh rd, offset(rs1)      |  GR[rd] = SLU(Mem[GR[rj] + imm]) </br> x[rd] = SLU((M[x[rs1] + sext(offset)]))      |          加载半字          |
|   ld.b rd, rj, imm   |     lb rd, offset(rs1)      |  GR[rd] = SLU(Mem[GR[rj] + imm]) </br> x[rd] = SLU((M[x[rs1] + sext(offset)]))      |          加载字节          |
|   ld.hu rd, rj, imm   |     lhu rd, offset(rs1)      |  GR[rd] = SLU(Mem[GR[rj] + imm]) </br> x[rd] = SLU((M[x[rs1] + sext(offset)]))      |          无符号加载半字          |
|   ld.bu rd, rj, imm   |     lbu rd, offset(rs1)      |  GR[rd] = SLU(Mem[GR[rj] + imm]) </br> x[rd] = SLU((M[x[rs1] + sext(offset)]))      |          无符号加载字节          |
|   st.w rd, rj, imm   |     sw rs2, offset(rs1)    | Mem[GR[rj] + imm] = GR[rd] </br> M[x[rs1] + sext(offset)] = x[rs2]      |          存储字          | 
|   st.h rd, rj, imm   |     sh rs2, offset(rs1)    | Mem[GR[rj] + imm] = SLU(GR[rd]) </br> M[x[rs1] + sext(offset)] = SLU(x[rs2])      |          存储半字          |
|   st.b rd, rj, imm   |     sb rs2, offset(rs1)    | Mem[GR[rj] + imm] = SLU(GR[rd]) </br> M[x[rs1] + sext(offset)] = SLU(x[rs2])      |          存储字节          |
| jirl rd, rj, *label* |   jalr rd, offset(rs1)   |   GR[rd] = pc + 4; pc = *label*  </br> x[rd] = pc+4; pc=(x[rs1]+sext(offset))&~1; |    间接相对跳转并链接    | 
|      b *label*       | j offset(jal x0, offset)（伪指令） | pc = *label*  </br> pc += sext(offset)       |        无条件跳转        | 
|      bl *label*      |      jal rd, offset      |   GR[1] = pc + 4; pc = *label*  </br> x[rd] = pc+4; pc += sext(offset) | 函数（子程序）调用并链接 | 
| beq rj, rd, *label*  |    beq rs1, rs2, offset     |   if (GR[rj] GR[rd]) pc = *label* </br> if (rs1 == rs2) pc += sext(offset)  |         相等跳转         | 
| bne rj, rd, *label*  |    bne rs1, rs2, offset     |   if (GR[rj] GR[rd]) pc = *label* </br> if (rs1 != rs2) pc += sext(offset) |         不等跳转         | 
| blt rj, rd, *label*  |    blt rs1, rs2, offset     | if (GR[rj] < GR[rd]) pc = *label* </br> if (rs1 < rs2) pc += sext(offset) |      有符号小于跳转      | 
| bge rj, rd, *label*  |    bge rs1, rs2, offset     | if (GR[rj] >= GR[rd]) pc = *label* </br> if (rs1 >= rs2) pc += sext(offset)|    有符号大于等于跳转    | 
| bltu rj, rd, *label* |    bltu rs1, rs2, offset    |   if (GR[rj] <u GR[rd]) pc = *label*  </br> if (rs1 <u rs2) pc += sext(offset)  |      无符号小于跳转      | 
| bgeu rj, rd, *label* |    bgeu rs1, rs2, offset    |   if (GR[rj] >=u GR[rd]) pc = *label* </br> if (rs1 >=u rs2) pc += sext(offset)  |    无符号大于等于跳转    | 