import { Outlet } from "react-router-dom";
import Navbar from "../ui/Navbar";

export default function Root(){
    return(
        <>
        <Navbar/>
        <Outlet/>
        </>
    )
}