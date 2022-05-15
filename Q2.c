typedef enum button_state{
    INIT,
    ON,
    PROTECTED,
    OFF,
    STATE_SIZE
}button_state_t;

button_state_t Current_State;
button_state_t Last_state;
unsigned char __button_pressed;
unsigned char seconds;

static void IRQ_timer(void);
void state_machine(void);
void init_micro(void);

#pragma vector=TMR0_VECTOR
static__interrupt void IRQ_timer()
{
    CLRTMR0IF = 0; //clear interrupt
    TMR0H = 0x10; //set timer values for 1s Tick
    TMR0L = 0x00;
    
    seconds++;

    if(seconds > 0x0A){ //check if TMR0 has reach 10 seconds
        TMRON = 0; //disable timer
    }
}


void main(void)
{
    init_micro();
    while(1)
    {
        state_machine();
        button_check();
    }
}

void button_check(void)
{
    //GPIO00; Variable containing current input pin state
    if(GPIO00){ //check current button state and if it was pressed set a flag
        __button_pressed = 1;
    }
}

void state_machine(void)
{
    switch (Current_State)
    {
        case INIT:
            Current_State = OFF; //start to OFF state
            Last_state = INIT;
            break;
        case OFF:
            //GPIO00; Variable containing current input pin state
            if(!GPIO00 && !TMR0ON && __button_pressed)//if button is not set and timer not set and button was pressed
            {
                TMR0ON = 1;
                seconds = 0;
                Current_State = PROTECTED;
                Last_state = OFF;
                __button_pressed = 0;
            }
            break;
        case PROTECTED:
            if(seconds > 0x0A)
            {
                if(Last_state == OFF)
                {
                    Current_State = ON;
                }
                else if(Last_state == ON)
                {
                    Current_State = OFF;
                }
            }
            break;
        case ON:
            //GPIO00; Variable containing current input pin state
            if(!GPIO00 && !TMR0ON && __button_pressed)//if button is not set and timer not set and button was pressed
            {
                TMR0ON = 1;
                seconds = 0;
                Current_State = PROTECTED;
                Last_state = ON;
                __button_pressed = 0;
            }
            break;    
        default:
            break;
    }
}

void init_micro(void){
    
    SETCLK = 0x100000; //Define clock, with prescaler and posscaler

    TMR0H = 0x10; //set timer values for 1s Tick
    TMR0L = 0x00;

    GPIO0INPUT = 0x01; //set io pin0_0 as Input
    GPIO00; //Variable containing current input pin state

    TMR0IE = 1; //Enable interrupt

    Current_State = INIT;
}