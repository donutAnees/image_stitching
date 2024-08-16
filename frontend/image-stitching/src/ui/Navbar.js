import { Link } from "react-router-dom";
import Navlist from "../components/Navlist";

export default function Navbar() {
  return (
      <section className="text-white flex justify-between items-center pt-4 px-6 md:px-10 fixed top-0 left-0 right-0 z-50 bg-transparent">
        <Link to={"/"} className="font-extrabold text-xl">Stitch</Link>
        <Navlist></Navlist>
      </section>
  );
}