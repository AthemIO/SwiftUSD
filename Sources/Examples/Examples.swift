/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import PixarUSD

@main
enum USDExamples {
    static func main() {
        Msg.logger.info("launched test program 'USDExamples'.")

        /* Setup all usd resources (python, plugins, resources). */
        Pixar.Bundler.shared.setup(.resources)

        Msg.logger.info("succesfully registered all usd plugins.")

        // custom ar resolver examples.
        ArResolverExamples.run()

        // scene description examples.
        SceneDescriptionExamples.run()

        // scene cache examples.
        // StageCacheExamples.run()  // Disabled - uses deprecated API

        // OpenExec examples
        // Now fixed: The System wrapper tracks all requests and ensures they are
        // destroyed before the system, preventing use-after-free crashes.
        OpenExecExamples.run()

        // OpenExec Benchmark - performance testing of the execution framework
        OpenExecBenchmark.run()

        Msg.logger.info("program completed succesfully, exiting...")
    }
}
