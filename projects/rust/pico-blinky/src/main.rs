#![no_std]
#![no_main]

use cortex_m_rt::entry;
use panic_halt as _;
use rp2040_hal::{pac, watchdog::Watchdog, clocks::init_clocks_and_plls, sio::Sio, gpio::Pins};

#[entry]
fn main() -> ! {
    let mut pac = pac::Peripherals::take().unwrap();
    let core = pac::CorePeripherals::take().unwrap();

    let mut watchdog = Watchdog::new(pac.WATCHDOG);
    let clocks = init_clocks_and_plls(
        12_000_000,
        pac.XOSC,
        pac.CLOCKS,
        pac.PLL_SYS,
        pac.PLL_USB,
        &mut pac.RESETS,
        &mut watchdog,
    ).ok().unwrap();

    let sio = Sio::new(pac.SIO);
    let pins = Pins::new(pac.IO_BANK0, pac.PADS_BANK0, sio.gpio_bank0, &mut pac.RESETS);
    let mut led_pin = pins.led.into_push_pull_output();

    loop {
        led_pin.set_high().unwrap();
        cortex_m::asm::delay(500_000);
        led_pin.set_low().unwrap();
        cortex_m::asm::delay(500_000);
    }
}