#![no_std]
#![no_main]
#![allow(nonstandard_style, dead_code)]

use aya_ebpf::{
    bindings::xdp_action, 
    macros::{xdp,map}, 
    maps::HashMap,
    programs::XdpContext
};
use aya_log_ebpf::info;

use core::mem;
use network_types::{
    eth::{EthHdr, EtherType},
    ip::{Ipv4Hdr, IpProto},
    tcp::TcpHdr,
    udp::UdpHdr,
};

#[xdp]
pub fn aya_demo(ctx: XdpContext) -> u32 {
    match try_aya_demo(ctx) {
        Ok(ret) => ret,
        Err(_) => xdp_action::XDP_ABORTED,
    }
}

#[map]
static BLOCKLIST: HashMap<u32, u32> = HashMap::<u32, u32>::with_max_entries(1024,0);

#[inline(always)]
fn 
ptr_at<T>(ctx: &XdpContext, offset: usize) -> Result<*const T, ()>
{
    let start = ctx.data();
    let end = ctx.data_end();
    let len = mem::size_of::<T>();
    if start + offset + len > end 
    {
        return Err(());
    }
    Ok((start + offset) as *const T)
}

fn
block_ip(address: u32) -> bool 
{
    unsafe { BLOCKLIST.get(&address).is_some() }
}

#[xdp]
pub fn xdp_firewall(ctx: XdpContext) -> u32
{
    match try_xdp_firewall(ctx) 
    {
        Ok(ret) => ret,
        Err(_) => xdp_action::XDP_ABORTED,
    }
}

fn try_xdp_firewall(ctx: XdpContext) -> Result<u32, ()> 
{
    let ethhdr: *const EthHdr = ptr_at(&ctx, 0)?;
    match unsafe { (*ethhdr).ether_type } 
    {
        EtherType::Ipv4 => {}
        _ => return Ok(xdp_action::XDP_PASS),
    }
    let ipv4hdr: *const Ipv4Hdr = ptr_at(&ctx, EthHdr::LEN)?;
    let source_addr = u32::from_be(unsafe {(*ipv4hdr).src_addr});

    let source_port = match unsafe {(*ipv4hdr).proto } 
    {
        IpProto::Tcp => {
            let tcphdr: *const TcpHdr = ptr_at(&ctx, EthHdr::LEN + Ipv4Hdr::LEN)?;
            u16::from_be(unsafe { (*tcphdr).source })
        }
        IpProto::Udp => {
            let udphdr: *const UdpHdr = ptr_at(&ctx, EthHdr::LEN + Ipv4Hdr::LEN)?;
            u16::from_be(unsafe { (*udphdr).source })
        }
        _ => return Err(()),
    };
    let action = 
        if block_ip(source_addr) 
        {
            xdp_action::XDP_DROP 
        } 
        else 
        {
            xdp_action::XDP_PASS
        };
    info!(&ctx, "source ip: {:i}, source port: {}, action: {}", source_addr, source_port, action);
    Ok(action)
}

fn try_aya_demo(ctx: XdpContext) -> Result<u32, u32> {
    info!(&ctx, "received a packet");
    Ok(xdp_action::XDP_PASS)
}

#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    unsafe { core::hint::unreachable_unchecked() }
}


