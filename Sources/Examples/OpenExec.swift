/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import PixarUSD

/*
 * OpenExec Examples
 *
 * NOTE: Temporarily disabled due to namespace resolution issues between
 * the C++ Usd module and the Swift Usd enum when using ExecUsd APIs.
 * The Usd.Stage.createNew() method is not accessible from this context.
 * This needs investigation into the module import chain.
 */

public enum OpenExecExamples
{
  public static func run()
  {
    Msg.logger.info("OpenExec examples temporarily disabled - see OpenExec.swift for details")
  }
}
