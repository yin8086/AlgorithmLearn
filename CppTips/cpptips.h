namespace Yincpp
{
    // test the usage of ( !! )
    void testDoubleExclamation()
    {
        bool bval1 = !5, bval2 = !!5;
        int  ival1 = !5, ival2 = !!5;

        std::cout << "Bool: [!5]=" << bval1 << " [!!5]=" << bval2 << std::endl;
        std::cout << "Int: [!5]=" << ival1 << " [!!5]=" << ival2 << std::endl;
        bool bval3 = !0, bval4 = !!0;
        int  ival3 = !0, ival4 = !!0;

        std::cout << "Bool: [!0]=" << bval3 << " [!!0]=" << bval4 << std::endl;
        std::cout << "Int: [!0]=" << ival3 << " [!!0]=" << ival4 << std::endl;

        bool bval5 = !(-5), bval6 = !!(-5);
        int  ival5 = !(-5), ival6 = !!(-5);

        std::cout << "Bool: [!-5]=" << bval5 << " [!!-5]=" << bval6 << std::endl;
        std::cout << "Int: [!-5]=" <<  ival5 << " [!!-5]=" << ival6 << std::endl;

    }
}